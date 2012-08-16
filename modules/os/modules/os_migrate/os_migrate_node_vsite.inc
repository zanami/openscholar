<?php 

class VsiteNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('vsite', 'vsite'); //current types are vsite, article, and page.  lets start with one of those instead?
    //a separate import for projects?
    $this->description = t('OS vsites');    
    $this->dependencies = preg_grep('/^VsiteNodeOS$/', $this->dependencies, PREG_GREP_INVERT);    
    
    //$this->addSimpleMappings(array('purl'));
    
    $this->addFieldMapping('field_site_description', 'og_description');
    $this->addFieldMapping('field_site_description:language', 'og_language')->defaultValue('und');
    $this->addFieldMapping('field_site_description:format')->defaultValue('plain_text');

    $this->addFieldMapping('field_site_address', 'field_vsite_address_value');
    $this->addFieldMapping('field_site_address:language', 'og_language')->defaultValue('und');
    $this->addFieldMapping('field_site_address:format')->defaultValue('plain_text');
    
    $this->addFieldMapping('group_access', 'og_private')->defaultValue(0);
    $this->addFieldMapping('group_group')->defaultValue(1);
    
    $this->addFieldMapping('field_site_logo', 'field_site_logo')->defaultValue(NULL);
    $this->addSimpleMappings(parent::subfields('field_site_logo', array('alt', 'title', 'cropbox_x', 'cropbox_y', 'language')));
    $this->addFieldMapping('field_site_logo:cropbox_width',' field_site_logo:cropbox_width')->defaultValue(200);
    $this->addFieldMapping('field_site_logo:cropbox_height',' field_site_logo:cropbox_height')->defaultValue(150);
         
    $this->addUnmigratedDestinations(parent::subfields('field_site_logo', array('file_class', 'destination_dir', 'destination_file', 'file_replace', 'preserve_files')), t('DNM'));
    $this->addFieldMapping('field_site_logo:source_dir')->defaultValue(variable_get('os_migrate_files', ''));
    
    //undo what nodeabstract has done
    $unmap = array_merge(array('body', 'group_nid', 'og_group_ref',),  
        parent::subfields('body', array('format', 'language', 'summary')));
    foreach ($unmap as $destination_field) {   
      $this->removeFieldMapping($destination_field);
    }
    
    $this->addUnmigratedSources(array('value', 'provider', 'format'), t('DNM'));
    $this->addUnmigratedSources(parent::subfields('field_vsite_logo', array('fid','list','data'), '_'));
  }
  
  public function query() {
    $query = parent::d6_db_select('node', 'n')->fields('n');
    $query->condition('n.type', 'vsite', '=');

    $query->join('content_type_vsite', 'ct', 'n.nid = ct.nid');
    $query->fields('ct', parent::subfields('field_vsite',array('address_value', 'logo_fid', 'logo_list', 'logo_data'), '_')); 
    $query->isNotNull('field_vsite_logo_fid'); //uncomment to test logo presence
    
    
    $query->join('purl', 'p', 'n.nid = p.id');
    $query->fields('p', array('value', 'provider'));
    //$query->condition('p.provider', 'spaces_og', '='); //domain sites had an extra entry for vsite_domain.  add those elsewehre later

//     $query->join('og_ancestry', 'oga', 'n.nid = oga.nid'); //node membership
//     $query->fields('oga', array('group_nid'));
    
    $query->join('og', 'og', 'n.nid = og.nid');
    $query->fields('og', parent::subfields('og', array('description', 'language', 'private'), '_'));    
    
    return $query;
  }
   function prepareRow($row) {
     if (isset($row->field_vsite_logo_fid)) {
       $file = parent::get_file_by_fid($row->field_vsite_logo_fid);
       //attach logo
       
       //$fields = array('language' => 'language', 'description' => 'description', 'list' => 'display');
//       parent::os_migrate_prepare_files_v24($result, $current_row, 'media_gallery_media', $fields);
       parent::os_migrate_prepare_files(array($file), $row, 'field_site_logo');
       $row->{'field_site_logo:langauge'} = $row->language;
       //$current_row->field_site_logo = $file->filename;
       
       if (strlen($row->field_vsite_logo_data) && ($data = unserialize($row->field_vsite_logo_data))) {
         $row->{'field_site_logo:alt'} = isset($data['alt']) ? $data['alt'] : '' ;
         $row->{'field_site_logo:title'} = isset($data['title']) ? $data['title'] : '';
         
         //add image dimensions, or defaults as needed
         if (isset($data['crop']['x'], $data['crop']['y'])) {
           $row->{'field_site_logo:cropbox_x'} = (int)$data['crop']['x'];
           $row->{'field_site_logo:cropbox_y'} = (int)$data['crop']['y'];
         } else {
           $row->{'field_site_logo:cropbox_x'} = 0;
           $row->{'field_site_logo:cropbox_y'} = 0;
         }
                     
         if (isset($data['crop']['height'], $data['crop']['width'])) {
           $row->{'field_site_logo:cropbox_height'} = min((int)$data['crop']['height'], 150);
           $row->{'field_site_logo:cropbox_width'} = min((int)$data['crop']['width'], 200);
         } else {
           //assume defaults
           $row->{'field_site_logo:cropbox_height'} = 150;
           $row->{'field_site_logo:cropbox_width'} = 200;
         }
       } else {
         //image provided, but no data.  
         $row->{'field_site_logo:cropbox_height'} = 150;
         $row->{'field_site_logo:cropbox_width'} = 200;
         $row->{'field_site_logo:cropbox_x'} = 0;
         $row->{'field_site_logo:cropbox_y'} = 0;
       }
       
     } else {
       $row->field_vsite_logo = null;
     }
     
     return;
   } 
  
   function prepare($entity, $current_row) {
     //purl isn't a valid destination yet, so just attach this to the entity.
     $entity->purl = array(
         'value' => $current_row->value,
         'provider' => $current_row->provider,
         'id' => $current_row->nid,
     );
   }

}