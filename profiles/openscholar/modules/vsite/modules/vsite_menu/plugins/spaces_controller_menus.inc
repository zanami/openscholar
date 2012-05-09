<?php

/**
 * Spaces controller for boxes module.
 */
class spaces_controller_menus extends spaces_controller {

  // Override of load_original_values
  protected function load_original_values($id = NULL) {
    if (empty($this->loaded_all['original'])) {
      
      if (!isset($id)) {
      	ctools_include('menu','os');
  
        $menus = array();
        foreach (os_get_menus() as $menu_name => $title) {
          $menus[$menu_name] = menu_tree_page_data($menu_name);
        }
        $this->values['original'] = $menus;
        $this->loaded_all['original'] = TRUE;
      }
      else if (!isset($this->loaded['original'][$id])) {
        $this->values['original'][$id] = menu_tree_page_data($id);
        $this->loaded['original'][$id] = TRUE;
      }
    }
  }
  
  // Override of load_preset_values
  protected function load_preset_values($id = NULL) {
    if (!$this->loaded_all['preset']) {
      parent::load_preset_values($id);
    	
    	//Do some post processing to the menu to replace menu's db functions
    	if(is_array($this->values['preset'])){
    		foreach ($this->values['preset'] as $menu_name => $a_menu_links) {
    			$this->values['preset'][$menu_name] = $this->tree_page_data($menu_name,$a_menu_links);
    		}
    	}
      $this->loaded_all['preset'] = TRUE;
    }
  }
  
  /**
   * DB Free copy of menu_tree_page_data
   * @param unknown_type $menu_name
   * @param unknown_type $menu_links
   */
  protected function tree_page_data($menu_name,$menu_links){
  	$tree = &drupal_static('spaces_controller_menus-tree_page_data', array());

	  // Check if the active trail has been overridden for this menu tree.
	  $active_path = menu_tree_get_path($menu_name);
	  // Load the menu item corresponding to the current page.
	  if ($item = menu_get_item($active_path)) {
	    $max_depth = MENU_MAX_DEPTH;
	    
	    // Generate a cache ID (cid) specific for this page.
	    $cid = 'links:' . $menu_name . ':page:' . $item['href'] . ':' . $GLOBALS['language']->language . ':' . (int) $item['access'] . ':' . (int) $max_depth .':' . hash('sha256', serialize($menu_links));

	    if (!isset($tree[$cid])) {
	      
	      $tree_parameters = array(
	        'min_depth' => 1,
	        'max_depth' => $max_depth,
	        'expanded' => array(0 => 0),
	        'active_trail' => array(0 => 0),
	      );
	
	      // Build the tree using the parameters; the resulting tree will be cached
        $data = $this->menu_build_tree($menu_name, $tree_parameters, $menu_links, $cid);
        
			  // Check access for the current user to each item in the tree.
			  menu_tree_check_access($data['tree'], $data['node_links']);
			  $tree[$cid] = $data['tree'];
	    }
	    return $tree[$cid];
	  }
	
	  return array();
  }
  
  /**
   * copy of menu_build_tree (menu.inc)
   * @param string $menu_name
   * @param array $tree_parameters
   * @param array $menu_links
   * @param string $cid
   */
  protected function menu_build_tree($menu_name, $tree_parameters, $menu_links, $cid){
    $cache = cache_get($tree_cid, 'cache_menu');
    if ($cache && isset($cache->data)) {
      return $cache->data;
    }
    
    $links = array();
    foreach ($menu_links as $mlid => $ml) {
    	// May not return data if it is an external link
	    $query = db_select('menu_router', 'm', array('fetch' => PDO::FETCH_ASSOC));
	    $query->addTag('translatable');
	    $query->fields('m', array(
	      'load_functions',
	      'to_arg_functions',
	      'access_callback',
	      'access_arguments',
	      'page_callback',
	      'page_arguments',
	      'delivery_callback',
	      'tab_parent',
	      'tab_root',
	      'title',
	      'title_callback',
	      'title_arguments',
	      'theme_callback',
	      'theme_arguments',
	      'type',
	      'description',
	    ));
	    $query->condition('m.path', $ml['router_path']);
    	
	    $item = $query->execute();
	    if(is_array($item)) $ml = array_merge($item,$ml);
	    
	    $links[] = $ml;
    }

    $data['tree'] = menu_tree_data($links);
    $data['node_links'] = array();
    menu_tree_collect_node_links($data['tree'], $data['node_links']);

    // Cache the data, if it is not already in the cache.
    cache_set($cid, $data, 'cache_menu');

    return $data;
  }

}
