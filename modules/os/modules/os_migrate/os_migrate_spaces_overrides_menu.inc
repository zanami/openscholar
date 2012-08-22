<?php 

class SpacesOverridesMenuMigration extends SpacesOverridesTableMigration {
  
  public function __construct() {
    $this->source_object ='vsite_menu'; 
    $this->dest_object = 'menus';
    parent::__construct();
  }
  
  public function query() {
    $query = parent::query();
    
    //there's no "Not in a menu" menu anymore.  Drop those items.
    //if there's time later, we could create a new menu for them, but not place the widget.
    $query->condition('object_id', 'none', '!=');
    return $query;
  }
  
  
  
  public function prepareRow(&$row) {
    $value = unserialize($row->value);
    $object_id = $row->object_id;
    
    $object_id = $this->_update_menu_name($object_id);
    
    
    //menu storage has changed a bit.  there's more info now.
    //biggest concern: mlid.  custom menu items forge it.  legit menu items interit it from menu_links.
    //os_menu_link_save <- reference
    
    //if forging, update vsite_menu_mlid_increment variable (conveniently located in spaces_overrides)
    
    //lets forge first.  then inherit if there's a good way to do so.
    $new_menu = array();
    foreach ($value as $menu_item) {
      $new_menu_item = $this->_update_menu_item($menu_item, $row->id);
      $id = $new_menu_item['mlid'];
      $new_menu[$id] = $new_menu_item;
    }    

    
    $row->object_id = $object_id;
    //$row->value = serialize($value);
    $row->value = serialize($new_menu);
    return parent::prepareRow($row);
  }
  
  public function _update_menu_name($name) {
    $menus = array(
      'features' => 'primary-menu',
      'none' => FALSE,
      'scholar_secondary_menu' => 'secondary-menu',
    );
    
    return (isset($menus[$name])) ? $menus[$name] : FALSE;
  }
  
  public function _update_menu_item($menu_item, $gid) {
    //update name
    $menu_item['menu_name'] = $this->_update_menu_name($menu_item['menu_name']);
    $menu_item['module'] = 'menu';
    
    //make mlid
    $mlid = $this->_os_migrate_get_mlid($gid);
    
    //attach defaults missing from d6
    $menu_item += array(
      'customized' => 0,
      'updated' => 0,
      'hidden' => 0,
      'external' => 0,
      'mlid' => $mlid,
      'plid' => 0,
      'p1' => $mlid,
      'depth' => 0,
    );
    
    foreach (range(2,9) as $n) {
      $menu_item["p$n"] = 0;
    }
    
    
    //todo depth, plids
    
    //plid, p1-9, depth,
    
    
    
    
    return $menu_item; 
  }
  
  //get a new mlid.  update existing mlid counter
  public function _os_migrate_get_mlid($gid) {
    $result = db_select('spaces_overrides', 's')
      ->fields('s', array('value'))
      ->condition('id', $gid, '=')
      ->condition('object_type', 'variable', '=')
      ->condition('object_id', 'vsite_menu_mlid_increment', '=')
      ->execute();
    
    $update = FALSE;
    if ($result->rowCount()) {
      $update = TRUE;
      $mlid = unserialize($result->fetchField());
    } elseif (module_exists('vsite_menu')) {
      $mlid = VSITE_MENU_CUSTOM_MLID;
    } else {
      $mlid = 1000;
    }
    
    $mlid++;

    //drupal_write_record('spaces_overrides', $record, array('id', 'object_type', 'object_id', 'type'));
    $record = array('value' => serialize($mlid), 'id' => $gid, 'object_type' => 'variable', 'object_id' => 'vsite_menu_mlid_increment', 'type' => 'og');
    
    if ($update) {
      $updated = db_update('spaces_overrides')
        ->fields($record)
        ->condition('id', $gid, '=')
        ->condition('object_type', 'variable', '=' )
        ->condition('object_id','vsite_menu_mlid_increment', '=')
        ->condition('type', 'og', '=')
        ->execute();
    } else {
      db_insert('spaces_overrides')->fields($record)->execute();
    }
    
    return $mlid;
  }
}