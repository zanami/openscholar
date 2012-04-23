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
    parent::__construct();

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
      $msg = t('Unable to connect to legacy database %db.  Please !link', array(
      				  '%db' => $os_migrate_database,
      				  '!link'=>l('check settings','admin/config/os_migrate')));
      drupal_set_message($msg, 'error');

      throw $e;
    }
  }


  /**
   * Gets files owned by node revision
   */
  function os_migrate_get_file_uploads($vid) {
    $output = array();

    // {upload}
    $query = self::d6_db_select('upload', 'u')
      ->fields('u', array('fid', 'vid', 'description', 'list'))
      ->condition('u.vid', $vid, '=');
    $query->join('files', 'f', 'u.fid = f.fid');
    $query->addField('f', 'filepath');
    $query->addField('f', 'timestamp');
    $query->orderBy('u.fid', 'ASC');
    $result = $query->execute();

    foreach ($result as $row) {
      $file = array(
        'path' => variable_get('os_migrate_files', FALSE) . '/' . $row->filepath,
        'description' => $row->description,
        'display' => $row->list,
        'timestamp' => $row->timestamp,
      );
      $output[] = drupal_json_encode($file);
    }


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

  //adds joins and fields based on migrate object's cck_fields property
  function addQueryCCK(&$query, $content_type, $table=NULL) {
    $table = ($table) ? $table : 'content_type_' . $content_type;
    //class has Link Date Text
    $fields = array();
    foreach($this->cck_fields as $name => $field) {
      switch ($field) {
        case 'Link':
          $fields += array('field_'.$name.'_url', 'field_'.$name.'_title', 'field_'.$name.'_attributes');
          break;

        case 'Date':
        case 'Text':
          $fields[] = 'field_'.$name.'_value';
          break;

        default:
          break;
      }
    }

    if (count($fields)) {
      $query->leftjoin($table, 'ct', 'n.vid = ct.vid');
      $query->fields('ct', $fields);
    }
  }

  //perform common preparations for cck types
  function prepareRowCCK(&$row) {
    foreach($this->cck_fields as $name => $field) {
      switch ($field) {
        case 'Link':
          $attr = 'field_'.$name.'_attributes';
          if (strlen($row->$attr)) {
            $row->$attr = unserialize($row->$attr);
          }
          break;

        default:
          break;
      }
    }
  }


    
  //get the fids/urls/inline body for this node.
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
  
  function os_migrate_update_body_files(&$row, $fid) {
    //TODO
    $query = db_select('migrate_map_fileinlineos', 'm') 
      ->fields('m', array('destid1'))
      ->condition('sourceid1', $fid, '=')
      ->isNotNull(('destid1'));
    $result = $query->execute();
    $file = $result->fetchAssoc();
    if (!$file) {
      return; //files that didn't copy block this. 
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
        
        //these tags are not rigorously tested.  probably not applicable everywhere.
        $tag = array(
          'type' => 'media',
          'view_mode' => 'media_large', 
          'fid' => $entity->fid,
          'attributes' => array(
            'class' => 'media-image',
            'typeof' => 'foaf:Image',
          ),
        );
        $text = $dom->createTextNode('[['.drupal_json_encode($tag).']]');
        $span = $a->parentNode;
        $span->parentNode->replaceChild($text, $span);
        
        $update = TRUE;
      }
    }
    
    if ($update) {
      $row->body = $this->_os_migrate_update_body_clean($dom->saveHTML());
    }
  }
  
  //match current body with files imported from body text
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
            $tag = $this->_os_migrate_inline_tag('object-data', $data['destid1'], &$object);
            $text = $dom->createTextNode($tag);
            $object->parentNode->replaceChild($text, $object);
            
          } elseif ($object && ($embed = $object->getElementsByTagName('embed'))) {
            //embed objects
            if ($target_embed_src == $embed->item(0)->getAttribute('src')) {
              $update = TRUE;
              //can't convince wysiwyg to take these strings.  lets ignore that for now and update tag later.
              $tag = $this->_os_migrate_inline_tag('object-embed', $data['destid1'], &$object);
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
  
  //update body of $row to use $fid's new file entity
  function os_migrate_update_body_image(&$row, $fid) {
    $query = db_select('migrate_map_imageinlineos', 'm')
      ->fields('m', array('destid1'))
      ->condition('sourceid1', $fid, '=')
      ->isNotNull(('destid1'));
    $entity_id = $query->execute()->fetchAssoc();
    $entity = file_load($entity_id['destid1']);
    
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
        $tag = array(
          'type' => 'media',
          'view_mode' => 'media_original',
          'fid' => $entity->fid,
          'attributes' => array(
            'class' => 'media-image',
            'typeof' => 'foaf:Image',
          ),
        );
        $text = $dom->createTextNode('[['.drupal_json_encode($tag).']]');

        // replace the iframe with our new tag
        $img->parentNode->replaceChild($text, $img);
        $update = TRUE;
      }
    }
    
    if ($update) {
      $body = $dom->saveHTML();
      $body = preg_replace('/<!DOCTYPE.*?>/', '', $body); //remove doctype
      $body = preg_replace('/.*\<body\>/m', '', $body); //remove html, body
      $body = preg_replace('/<\/body>.*/m', '', $body); //remove closing tags
      $row->body = $body;
    }
    
  }
   /**
   * given a row (mainly nid and body) get new files.
   * inlinefiles -> fid  => migmap_fileinlineos  -> dest fid
   * inline_image -> fid => migmap_imageinlineos -> dest fid
   * 
   * inline_oembed -> nid => migmap_embedinlineos -> dest fid
   */
  //if this row has inline embeds, update them to refer to new file entities
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
    return '[[' . drupal_json_encode($tag) . ']]';
  }
  
  //body saved by dom is full document.  strip doctype, html, body tags.
  function _os_migrate_update_body_clean($body) {
    $body = preg_replace('/<!DOCTYPE.*?>/', '', $body); //remove doctype
    $body = preg_replace('/.*\<body\>/m', '', $body); //remove html, body
    $body = preg_replace('/<\/body>.*/m', '', $body); //remove closing tags
    return $body;
  }
  //update node body to use new file
/*  function os_migrate_update_body_text(stdClass &$file_entity, $body, $desc) {
    $dom = filter_dom_load($body);
    $uri = str_replace('public://', '', $file_entity->uri);
    
    $anchors = $dom->getElementsByTagName('a');
    for ($i=0, $len=$anchors->length; $i<$len; $i++) {
      $a = $anchors->item($i);
      $href = $a->getAttribute('href');
      if (strpos($href, $uri)) {
        //replace the anchor
        $tag = array(
          'type' => 'media',
          'view_mode' => 'media_original',
          'fid' => $file_entity->fid,
          'attributes' => array(
            'class' => 'media-image',
            'typeof' => 'foaf:Image',
          ),
        );
        $text = $dom->createTextNode('[['.drupal_json_encode($tag).']]');

        // replace the iframe with our new tag
        $a->parentNode->replaceChild($text, $a);
        break;
      }
    }
    return $dom->saveHTML();
  }
  */
  

  
  function os_migrate_wf_to_media($row) {
    $b = $row->body;
    // things I need to find:
    // iframes              pasted iframes - go into media_embed
    // object tags          old style oembeds - find the src. If it's external and not in media_embed's whitelist, go to oembed
    // script tags
    // images to local files  inline images - go to local file system
    // links to local files   inline files - local file system
    // [[wysiwyg_fields]] tags  new style oembeds - go into oembed
    $media_embed = array();
    $media_oembed = array();
    $media_local = array();

    // get a dom document
    $dom = filter_dom_load($b);

    // get the iframes
    // all of these are pasted
    $iframes = $dom->getElementsByTagName('iframe');
    for ($i=0,$l=$iframes->length; $i<$l; $i++) {
      $if = $iframes->item($i);
      $media_embed[] = $if;
    }

    // get the objects
    // these come from oembed pretty much exclusively
    $objects = $dom->getElementsByTagName('object');
    for ($i=0,$l=$objects->length; $i<$l; $i++) {
      $media_oembed[] = $objects[$i];
    }

    $scripts = $dom->getElementsByTagName('script');
    for ($i=0,$l=$scripts->length; $i<$l; $i++) {
      $media_oembed[] = $scripts[$i];
    }

    $imgs = $dom->getElementsByTagName('img');
    for ($i=0,$l=$imgs->length; $i<$l; $i++) {
      $url = $imgs[$i]->getAttribute('src');
      // check url to see if its to a local file
      if ($this->url_is_local($url)) {
        $media_local[] = $imgs[$i];
      }
    }

    $links = $dom->getElementsByTagName('a');
    for ($i=0,$l=$links->length; $i<$l; $i++) {
      $url = $links[$i]->getAttribute('href');

      // check if url is a file
      if ($this->url_is_local($url) && $this->url_is_file($url)) {
        $media_local[] = $links[$i];
      }
    }

    // replace all the dom elements with media tags
    foreach ($media_embed as $me) {
      // save the html as an html file

      // pretending its a form turns out to be the easiest way to create an entity from scratch
      // assuming there's already a form.
      // the form_state array is ugly but there's not much I can do about it
      $form_state = array(
        'submitted' => true,
        'programmed' => true,
        'input' => array(
          'field_html_code' => array(
            'und' => array(
              array(
                'value' => $dom->saveHTML($me),
              )
            ),
          ),
          'field_html_title' => array(
            'und' => array(
              array(
                'value' => 'Imported iframe code',
              )
            )
          ),
        ),
      );
      $form = drupal_build_form('media_embed_add', $form_state);
      // get the text to be inserted in the document
      $file = file_uri_to_object($form_state['file']->uri);

      // i can afford to be dumb about this
      // because there is no configuration that needs doing here
      // the only thing that changes between iframes is the fid
      $tag = array(
        'type' => 'media',
        'view_mode' => 'media_original',
        'fid' => $file->fid,
        'attributes' => array(
          'class' => 'media-image',
          'typeof' => 'foaf:Image',
        ),
      );
      $text = $dom->createTextNode('[['.drupal_json_encode($tag).']]');

      // replace the iframe with our new tag
      $me->parentNode->replaceChild($text, $me);
    }

    // oembed tags into links
    $form_state = array(
        'submitted' => true,
        'programmed' => true,
        'input' => array(
          'embed_code' => '',
        ),
      );
    foreach ($media_oembed as $node) {
      // get the url
      $html = $dom->saveHTML($node);

      foreach ($row->os_inline_oembed as $f) {
        $url = $f['field_os_inline_oembed'];
        if ($oembed = oembedcore_oembed_data($url) && $oembed->html == $html) {
          // this is the one we want
          $form_state['input']['embed_code'] = $url;
          drupal_build_form('media_internet_add', $form_state);
          $file = file_uri_to_object($form_state['file']->uri);

          $tag = array(
            'type' => 'media',
            'view_mode' => 'media_large',
            'fid' => $file->fid,
            'attributes' => array(
              'class' => 'media-image',
              'typeof' => 'foaf:Image',
            ),
          );
          $text = $dom->createTextNode('[['.drupal_json_encode($tag).']]');

          // replace the iframe with our new tag
          $node->parentNode->replaceChild($text, $node);
          break;
        }
      }
    }

    /** local files **/
    foreach ($media_local as $node) {
      $url = '';
      if ($url = $node->getAttribute('href')) {}
      elseif ($url = $node->getAttribute('src')) {}
      else { continue; }  // no url, found

      $file = file_uri_to_object($url, false);  // get a file from our old uri. this is a dumb function so this works
      $destination = '/path/to/the/file';    // TODO: Fill placeholder
      file_copy($file, $destination, false);    // copy the file
      $file = file_uri_to_object($file->uri);  // get the new file object so we cna make a tag out of it

      $tag = array(
        'type' => 'media',
        'view_mode' => $node->nodeName=='A'?'media_link':'media_large',
        'fid' => $file->fid,
        'attributes' => array(
          'class' => 'media-image',
          'typeof' => 'foaf:Image',
        ),
      );
      $text = $dom->createTextNode('[['.drupal_json_encode($tag).']]');

      // replace the old html tag with our new markdownish one
      $node->parentNode->replaceChild($text, $node);
    }

    // return the document to html
    $body = $dom->saveHTML();

    // run through the new html looking for wysiwyg_fields tags
    // replace them with media tags

    $pattern = '~\[wysiwyg_field wf_field="field_os_inline_oembed" wf_deltas="([,\d]+)" wf_formatter="([^"]+)" [^\]]*\]~';
    $matches = array();
    preg_match_all($pattern, $body, $matches, PREG_SET_ORDER);
    foreach ($matches as $m) {
      $deltas = explode(',', $m[1]);
      $delta = $deltas[0];

      if ($oembed = $row->os_inline_oembed[$delta]) {
        $form_state = array(
          'submitted' => true,
          'programmed' => true,
          'input' => array(
            'embed_code' => $oembed['field_os_inline_oembed_url'],
          ),
        );
        drupal_build_form('media_internet_add', $form_state);
        $file = file_uri_to_object($form_state['file']->uri);

        $tag = array(
          'type' => 'media',
          'view_mode' => 'media_large',  // do something with the formatter?
          'fid' => $file->fid,
          'attributes' => array(
            'class' => 'media-image',
            'typeof' => 'foaf:Image',
          ),
        );
        $text = '[['.drupal_json_encode($tag).']]';

        // replace the old tag with our new tag
        str_replace($m[0], $text, $body);
      }
    }

    $row->body = $body;
  }


}


