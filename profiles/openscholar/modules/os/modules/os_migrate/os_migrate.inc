<?php

/**
 * @class OSMigration
 *
 * Abstract class for all OpenScholar migration classes
 * Sets some defaults
 * Provides d6_db_select helper for getting d6 db for import
 */
abstract class OSMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first for basic setup
    $args = func_get_args();
    if (count($args)) {
      parent::__construct($args[0]);
    } else {
      parent::__construct();
    }
    
    // With migrate_ui enabled, migration pages will indicate people involved in
    // the particular migration, with their role and contact info. We default the
    // list in the shared class; it can be overridden for specific migrations.
    $this->team = array(
      new MigrateTeamMember('Jon Sagotsky', 'jsagotsky@fas.harvard.edu', t('Migration Lead')),
    );

    // Individual mappings in a migration can be linked to a ticket or issue
    // in an external tracking system. Define the URL pattern here in the shared
    // class with ':id:' representing the position of the issue number, then add
    // ->issueNumber(1234) to a mapping.
    $this->issuePattern = 'https://redmine.hmdc.harvard.edu/issues/:id:';
  }

  /**
   * @function d6_db_select
   *
   * As db_select, but targets old d6 database
   * Sends users to admin page on a misconfigured db
   */
  public function d6_db_select($target, $alias = NULL, array $options = array()) {
    static $os_migrate_database;
    if (!isset($os_migrate_database)) {
      $os_migrate_database =  variable_get('os_migrate_database', '');
    }

    try {
      return Database::getConnection('default', $os_migrate_database)
        ->select($target, $alias, $options);
    } catch (DatabaseConnectionNotDefinedException $e) {
      flood_register_event('os_migrate_bad_db');
      $msg = t('Unable to connect to legacy database %db  Please !link', array(
      				  '%db' => $os_migrate_database . '.',
      				  '!link'=>l('check settings','admin/config/os_migrate')));
      drupal_set_message($msg, 'error');

      throw $e;
    }
  }

  /**
   * @function os_migrate_prepare_files
   * 
   * Helper function for translating db results to array for file field. 
   * 
   * @param DatabaseStatementBase $result
   * 	Results from query on files table.  Requires filepath, description, list, and timestamp.
   * 
   * @return array
   */
  function os_migrate_prepare_files($result) {
    $output = array();
    foreach ($result as $row) {
      $file = array(
        'path' => variable_get('os_migrate_files', FALSE) . '/' . $row->filepath,
        'description' => $row->description,
        'display' => $row->list,
        'timestamp' => $row->timestamp,
      );
      $output[] = drupal_json_encode($file);
    }
    return $output;
  }
  
  /**
   * Gets files owned by node revision
   */
  function os_migrate_get_file_uploads($vid) {

    // {upload}
    $query = self::d6_db_select('upload', 'u')
      ->fields('u', array('fid', 'vid', 'description', 'list'))
      ->condition('u.vid', $vid, '=');
    $query->join('files', 'f', 'u.fid = f.fid');
    $query->addField('f', 'filepath');
    $query->addField('f', 'timestamp');
    $query->orderBy('u.fid', 'ASC');
    $result = $query->execute();

    /*
    foreach ($result as $row) {
      $file = array(
        'path' => variable_get('os_migrate_files', FALSE) . '/' . $row->filepath,
        'description' => $row->description,
        'display' => $row->list,
        'timestamp' => $row->timestamp,
      );
      $output[] = drupal_json_encode($file);
    }*/
    $output = $this->os_migrate_prepare_files($result);


    // {content_field_os_inline_files} - filefield
    // os_inline_files is going to take some extra processing.  matt is handling it elsewhere.  cck inline files go here.
    // they end up filefields
    foreach (array('presentation_file') as $field) {
      $table = 'content_field_' . $field;

      $query = self::d6_db_select($table, 'ff')
        ->fields('ff', array('vid', 'nid', 'delta', 'field_'.$field.'_fid', 'field_'.$field.'_list', 'field_'.$field.'_data'))
        ->condition('ff.vid', $vid, '=');
      $query->join('files', 'f', 'ff.field_'.$field.'_fid = f.fid');
      $query->addField('f', 'filepath');
      $query->addField('f', 'timestamp');
      $query->orderBy('f.fid', 'ASC');
      $result = $query->execute();

      foreach ($result as $row) {
        $ff_data = unserialize($row->{'field_'.$field.'_data'});
        $file = array(
          'path' => variable_get('os_migrate_files', FALSE) . '/' . $row->filepath,
          'description' => $ff_data['description'],
          'display' => ($row->{'field_'.$field.'_list'}),
          'timestamp' => $row->timestamp,
        );
        $output[] = drupal_json_encode($file);
      }
    }

    //also get inline images, software packages, pubs?

    return $output;
  }
  
  /*
   * Get variable from d6.  Cache var.
   */
  function d6_variable_get($var, $default=NULL) {
    static $variable_cache;
    
    if (!isset($variable_cache)) {
      $variable_cache = array();
    }
    
    if (!isset($variable_cache[$var])) {
      $query = $this::d6_db_select('variable', 'v')
        ->fields('v', array('value'))
        ->condition('v.name', $var, '=');
      $value = unserialize($query->execute()->fetchField());
      $variable_cache[$var] = (isset($value)) ? $value : $default;
    }
    
    return $variable_cache[$var];
  }

  /*
   * Check that a file exists before letting migrate try to copy it.
   * Inline images and files have been looping when a file isn't available.
   * This helper function processes the path in the same way before trying to
   * copy so we can bail out if the file is unavailable
   */
  function legacy_file_exists($path, $log_error=TRUE) {
//  	$path = trim(parse_url($path, PHP_URL_PATH), '/');
    $path = str_replace(' ', '%20', $path);

    //try file first
    if (file_exists($path)) {
    	return TRUE;
    } 
    
    //now try url
    $headers = @get_headers($path);
    if ($headers && ($headers[0] != 'HTTP/1.0 404 Not Found')) {
    	return TRUE;
    }
    
    if ($log_error) {
  	  self::saveMessage(t('Could not copy inline file %uri.  Ignoring it.', array('%uri'=>$path)));
    }
    return FALSE;
  }
  /*************************************
   * CCK Field mappers
   ***************************************/
  
  //adds joins and fields based on migrate object's cck_fields property
  function addQueryCCK(&$query, $content_type) {
    $table = 'content_type_' . $content_type;

    //class has Link Date Text
    $fields = array();
    $leftjoin = array();
    foreach($this->cck_fields as $value) {
      $field = $value['cck_type'];
      $name = $value['cck_name'];
      //add a table override as well?
      
      switch ($field) {
        case 'Link':
          $fields += array('field_'.$name.'_url', 'field_'.$name.'_title', 'field_'.$name.'_attributes');
        break;

        case 'Date':
        case 'Text':
          $fields[] = 'field_'.$name.'_value';
        break;
          
        case 'File':
          $fields += array("field_${name}_fid", "field_${name}_list", "field_${name}_data");   
          $leftjoin[] = array('table'=>'files', 'alias'=>'f', 'join'=>"ct.field_${name}_fid = f.fid",
            'fields'=>array('filepath',));
          
        break;

        default:
          break;
      }
    }

    if (count($fields)) {
      $query->leftjoin($table, 'ct', 'n.vid = ct.vid');
      $query->fields('ct', $fields);
    }
    //would do additional joins in case, but ct hasn't been added yet, so do them here instead.
    foreach ($leftjoin as $join) {
      $query->leftjoin($join['table'], $join['alias'], $join['join']);
      $query->fields($join['alias'], $join['fields']);
    }
  }

  //perform common preparations for cck types
  function prepareRowCCK(&$row) {
    foreach($this->cck_fields as $value) {
      $field = $value['cck_type'];
      $name = $value['cck_name'];
      
      switch ($field) {
        case 'Link':
          $attr = 'field_'.$name.'_attributes';
          if (strlen($row->$attr)) {
            $row->$attr = unserialize($row->$attr);
          }
          
        break;
        
        case 'File':
          if (isset($row->field_announcement_photo_fid)) {
            $row->field_photo = variable_get('os_migrate_files', '') . '/' . $row->filepath;
          }
        break;
          

        default:
          break;
      }
    }
  }
  
  //map common cck fields to d7 fields
  function addMappingsCCK() {
    foreach($this->cck_fields as $value) {
      $cck_type = $value['cck_type'];
      $cck_name = $value['cck_name'];
      $dest_name = $value['dest_name'];
      $dest_type = $value['dest_type'];  //whole name or just the common part?  field_photo implies whole name...
      
      switch ($dest_type) {
        case 'Link':
          $field = 'field_'.$dest_name.'_link';
          $cck = 'field_'.$cck_name . '_url';
          // $arguments = MigrateLinkFieldHandler::arguments($fields);
          //pass info from fields to arguments[] in field_link's prepare()
          $this->addFieldMapping($field, $cck)->arguments(array( 
            'url' =>        array('source_field' => 'field_'.$cck_name.'_url'),
            'title' =>      array('source_field' => 'field_'.$cck_name.'_title'),
            'attributes' => array('source_field' => 'field_'.$cck_name.'_attributes'),
          ));    
        break;
        
        case 'Image':
          $this->addFieldMapping($dest_name, $dest_name)->arguments(array(
            'file_function' => 'file_copy',
            'file_source' => array('source_field'=>'filepath'),
          ));
        break;
      }
    }
  }
  
  function sourceFieldsCCK() {
    $return = array();
    foreach ($this->cck_fields as $value) {
      $name = $value['cck_name'];
      switch ($value['cck_type']) {
        case 'Link':
          $return += array(
            //name => desc
						'field_'.$name.'_url' => 'Link URL',
            'field_'.$name.'_title' => 'Link Title',
            'field_'.$name.'_attributes' => 'Link Attributes',
          );
        break;
        
        case 'File':
          $return += array(
            //name => desc
						'field_'.$name.'_fid' => 'File ID',
            'field_'.$name.'_list' => 'Show file?',
            'field_'.$name.'_data' => 'Metadata about file',
          );
        break;
      }
    }
    return $return;
  }


   /*************************
    * inline file processing
    *****************************/
  
    /**
     * @function os_migrate_list_inline_files
     * 
     * Gets all the inline files associated with a nid.  Also checks if node bodies
     * contain inline objects that may not be in inline table
     **/
    function os_migrate_list_inline_files($nid) {
  
    $query = $this->d6_db_select('node', 'n')
      ->condition('n.nid', $nid, '=');
    $query->join('content_field_os_inline_files', 'cff', 'n.nid = cff.nid');
    $query->fields('cff', array('field_os_inline_files_fid'));
    $query->join('content_field_os_inline_image', 'cfi', 'n.nid = cfi.nid');
    $query->fields('cfi', array('field_os_inline_image_fid'));
    $query->join('content_field_os_inline_oembed', 'cfu', 'n.nid = cfu.nid');
    $query->fields('cfu', array('field_os_inline_oembed_url'));
    $query->join('node_revisions', 'nr', 'n.nid = nr.nid');
    
    $query->condition(db_or()
      ->isNotNull('cff.field_os_inline_files_fid')
      ->isNotNull('cfi.field_os_inline_image_fid')
      ->isNotNull('cfu.field_os_inline_oembed_url')
      ->condition(db_or()
        ->condition('body', '%<iframe%', 'LIKE')
        ->condition('body', '%<object%', 'LIKE')
        ));
    
    $files = array();
    foreach ($query->execute() as $row) {
      $arr = array_filter((array)$row);
      if (!count($arr)) { $arr['body'] = TRUE; }      
      $files[] = $arr;
    }
    
    return $files;
  }    
  
  /**
   * @function os_migrate_update_body_files(&$row, $fid)
   * 
   * Given a node's row and the fid of an inline file in that node, update the body text to 
   * refer to the new file entity
   **/
  function os_migrate_update_body_files(&$row, $fid) {
    $query = db_select('migrate_map_fileinlineos', 'm') 
      ->fields('m', array('destid1'))
      ->condition('sourceid1', $fid, '=')
      ->isNotNull(('destid1'));
    $result = $query->execute();
    $file = $result->fetchAssoc();
    if (!$file) {
      return; 
    }
    
    $entity = file_load($file['destid1']);
    $uri = explode('/',$entity->uri);
    $uri = array_slice($uri, 3);
    $uri = implode('/', $uri);
      
    $dom = filter_dom_load($row->body);
    $links = $dom->getElementsByTagName('a');
    $update = FALSE;
    for ($i=0,$len=$links->length; $i<$len; $i++) {
      $link = $links->item($i);
      if ($link && strpos($link->getAttribute('href'), $uri)) {
        $update = TRUE;
        $tag = $this->_os_migrate_inline_tag('file', $entity->fid, $link);
        $text = $dom->createTextNode($tag);
        $link->parentNode->replaceChild($text, $link);
      }
    }
    
    if ($update) {
      $row->body = $this->_os_migrate_update_body_clean($dom->saveHTML());
    }
  }
  
  /**
   * @function os_migrate_update_body_url(&$row, $url)
   * 
   * Given a node's row and the url of an inline link in that node, update the body text to 
   * refer to the new file entity
   **/
  function os_migrate_update_body_url(&$row, $url) {
    $query = db_select('migrate_map_embedinlineos', 'm')
      ->fields('m', array('destid1'))
      ->condition('sourceid1', $row->vid, '=');
      
    $entity_id = $query->execute()->fetchAssoc();
    $entity = file_load($entity_id['destid1']);
    
    $dom = filter_dom_load($row->body);
    //check all a tags.  we'll have to replace parent though since its a space for the oembed
    $anchors = $dom->getElementsByTagName('a');
    $update = FALSE;
    for ($i=0, $len=$anchors->length; $i<$len; $i++) {
      $a = $anchors->item($i);
      if (isset($a) && ($a->getAttribute('href') == $url)) {
        $tag = $this->_os_migrate_inline_tag('url', $entity->fid, $a);
        $text = $dom->createTextNode($tag);
        $span = $a->parentNode;
        $span->parentNode->replaceChild($text, $span);
        
        $update = TRUE;
      }
    }
    
    if ($update) {
      $row->body = $this->_os_migrate_update_body_clean($dom->saveHTML());
    }
  }
  
  /**
   * @function os_migrate_update_body_body(&$row, $fid)
   * 
   * Given a node's row and the fid of a file extracted from the body of the node, update the body text to 
   * refer to the new file entity
   **/
  function os_migrate_update_body_body(&$row, $fid) {
    $query = db_select('os_migrate_body_inline_objects', 'o')
      ->fields('o', array('html'))
      ->condition('vid', $row->vid, '=');
    $query->join('migrate_map_htmlinlineos', 'm', 'o.id = m.sourceid1');
    $query->fields('m', array('destid1'));
    $query->isNotNull('m.destid1');
    
    $data = $query->execute()->fetchAssoc();
    $update = FALSE;
    
    //lets try iframes first
    $target_dom = filter_dom_load($data['html']);
    $target_iframes = $target_dom->getElementsByTagName('iframe');
    if ($target_iframes->length) {
      $target_src = $target_iframes->item(0)->getAttribute('src');
      
      $dom = filter_dom_load($row->body);
      $iframes = $dom->getElementsByTagName('iframe');
      for ($i=0,$len=$iframes->length; $i<$len; $i++) {
        $iframe = $iframes->item($i);
        if ($iframe && ($src = $iframe->getAttribute('src'))) {
          if ($src == $target_src) {
            $update = TRUE;
            $tag = $this->_os_migrate_inline_tag('iframe', $data['destid1'], $iframe); 
            $text = $dom->createTextNode($tag);
            $iframe->parentNode->replaceChild($text, $iframe);     
          }
        }
      }
    }
    
    //and objects.  some have data.  others have children embeds.
    $target_objects = $target_dom->getElementsByTagName('object');
    if ($target_objects->length) {
      $target_object = $target_objects->item(0);
      $target_data = $target_object->getAttribute('data');
      $target_embed = $target_object->getElementsByTagName('embed');
      if ($target_embed->length) {
        $target_embed_src = $target_embed->item(0)->getAttribute('src');
      }
      
      $dom = filter_dom_load($row->body);
      $objects = $dom->getElementsByTagName('object');
      for ($i=0,$len=$objects->length; $i<$len; $i++) {
        $object = $objects->item($i);
        if ($object && ($src = $object->getAttribute('data'))) {
          //data objects - currently untested.
          if ($target_data == $src) {
            $update = TRUE;
            $tag = $this->_os_migrate_inline_tag('object-data', $data['destid1'], $object);
            $text = $dom->createTextNode($tag);
            $object->parentNode->replaceChild($text, $object);
            
          } elseif ($object && ($embed = $object->getElementsByTagName('embed'))) {
            //embed objects
            if ($target_embed_src == $embed->item(0)->getAttribute('src')) {
              $update = TRUE;
              //can't convince wysiwyg to take these strings.  lets ignore that for now and update tag later.
              $tag = $this->_os_migrate_inline_tag('object-embed', $data['destid1'], $object);
              $text = $dom->createTextNode($tag);
              $object->parentNode->replaceChild($text, $object);
            }
          }
        }
      }
    }
    
    if ($update) {
      $row->body = $this->_os_migrate_update_body_clean($dom->saveHTML());
    }

    return;
  }
  
  /**
   * @function os_migrate_update_body_image(&$row, $fid)
   * 
   * Given a node's row and the fid of an inline image in that node, update the body text to 
   * refer to the new file entity
   **/
  function os_migrate_update_body_image(&$row, $fid) {
    $query = db_select('migrate_map_imageinlineos', 'm')
      ->fields('m', array('destid1'))
      ->condition('sourceid1', $fid, '=')
      ->isNotNull(('destid1'));
    $entity_id = $query->execute()->fetchAssoc();
    if (! ($entity_id && $entity = file_load($entity_id['destid1']))) {
      self::displayMessage(t('Migration error on source file %fid.  Could not find file in migration tables.  Continuing without file.  Check Inline migrations.', array('%fid'=>$fid)), 'warning');
      return; 
    } 
    
    $uri = str_replace('public://', '', $entity->uri);
    $a = explode('/', $uri);
    $a = array_slice($a, 3);
    $uri = implode('/', $a);
    
    $dom = filter_dom_load($row->body);
    $images = $dom->getElementsByTagName('img');
    $update = FALSE;
    for ($i=0, $len=$images->length; $i < $len; $i++) {
      $img = $images->item($i); 
      if (isset($img) && strpos($img->getAttribute('src'), $uri)) {
        $tag = $this->_os_migrate_inline_tag('image', $entity->fid, $img);
        $text = $dom->createTextNode($tag);
        $img->parentNode->replaceChild($text, $img);
        $update = TRUE;
      }
    }
    
    if ($update) {
      $row->body = $this->_os_migrate_update_body_clean($dom->saveHTML());
    }
    
  }
  
  /**
   * @function os_migrate_inline_update
   * 
   * Check node row for inline embeds.  If any exist update body to use new file entities.
   **/
  function os_migrate_inline_update(&$current_row) {
    $files = $this->os_migrate_list_inline_files($current_row->nid);
    if (!count($files)) {
      return;
    }
    
    $file_funcs = array(
      'field_os_inline_image_fid' => 'image',
      'field_os_inline_files_fid' => 'files',
    	'field_os_inline_oembed_url' => 'url',
    	'body' => 'body',
    );
    
    foreach ($files as $file) {
      foreach ($file as $src => $fid) {
        $func = 'os_migrate_update_body_' . $file_funcs[$src];
        $this->$func($current_row, $fid);
      }
    }
    return;
  }
  
  //prepare the inline tag
  //generic for now.
  function _os_migrate_inline_tag($type, $fid, &$dom_object) {
    $tag = array(
      'type' => 'media',
      'view_mode' => 'media_original',
      'fid' => $fid,
      'attributes' => array(
        'class' => 'media-image',
        'typeof' => 'foaf:Image',
      ),
    );
        
    $sizes = array(
      'imagecache-inline_larger' => 'media_large',
      'imagecache-inline_default' => 'media_original',
      '' => 'media_large',
    );
    
    $attributes = array();
    
    switch ($type) {
      case 'image':
        //images may have titles but there's no way to enter a title in media
        $class = $dom_object->getAttribute('class');
        if (isset($sizes[$class])) {
          $tag['view_mode'] = $sizes[$class];
        }
        $attributes = array('width'=>'width', 'height'=>'height');
      break;
        
      case 'file':
      break;

      case 'url':
      break;
      
      case 'iframe':
        $attributes = array('width'=>'width', 'height'=>'height');
        //style attr?  keep or ditch
      break;
    }

    $tag['attributes'] += $this->_os_migrate_get_attributes($attributes, $dom_object);
    return ' [[' . drupal_json_encode($tag) . ']] ';
  }
  
  /**
   * get attributes from dom element
   * attr array maps old (d6) to new d7 attribute, just in case things change due to new schema
   **/
  function _os_migrate_get_attributes(array $attr, DOMElement $dom) {
    $return = array();
    foreach ($attr as  $old => $new) {
      if ($value = $dom->getAttribute($old)) {
        $return[$new] = $value;
      }
    }
    return $return;
  }
  
  //body saved by dom is full document.  strip doctype, html, body tags.
  function _os_migrate_update_body_clean($body) {
    $body = preg_replace('/<!DOCTYPE.*?>/', '', $body); //remove doctype
    $body = preg_replace('/.*\<body\>/m', '', $body); //remove html, body
    $body = preg_replace('/<\/body>.*/m', '', $body); //remove closing tags
    return $body;
  }
}


