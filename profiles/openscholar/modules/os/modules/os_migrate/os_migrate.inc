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
      $msg = t('Unable to connect to db %db.  Please !link', array(
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
  
  //get all inline embeds.  return array of them.
  function os_migrate_inline_oembed($vid) {
    $query = self::d6_db_select('content_field_os_inline_oembed', 'oe')
      ->fields('oe', array('vid', 'nid', 'delta', 'field_os_inline_oembed_url', 'field_os_inline_oembed_title', 'field_os_inline_oembed_attributes'))
      ->condition('oe.vid', $vid, '=')
      ->isNotNull('oe.field_os_inline_oembed_url');
    $result = $query->execute();

    $embeds = array();
    foreach ($result as $row) {
      $embeds[$row->delta] = $row;
    }

    return $embeds;
  }

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

  private function url_is_local($url) {
    global $base_url;
    return (strpos($url, $base_url) !== FALSE);
  }

  private function url_is_file($url) {

  }
}


