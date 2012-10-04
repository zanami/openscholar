<?php

class SpacesOverridesBoxesMediaMigration extends OSMigration {

  public function __construct() {
     parent::__construct();

     //spaces overrides has no primary key, so use these
     $key_schema = array(
       'id' => array(
         'type' => 'varchar',
         'length' => 255,
         'not null' => TRUE,
       ),
       'object_type' => array(
         'type' => 'varchar',
         'length' => 64,
         'not null' => TRUE,
       ),
       'object_id' => array(
         'type' => 'varchar',
         'length' => 255,
         'not null' => TRUE,
       ),
     );

     $fields = array('value'=>t('URL'), 'timestamp'=>t('Timestamp'), 'destination_file'=>t('Destination File'));

    //dpm(field_info_bundles('file'));

     //$this->destination = new MigrateDestinationMedia('html', 'MigrateFileBlob');
     $this->destination = new MigrateDestinationMedia('html', 'MigrateFileFid'); //create file, pass fid along

     $this->description = t('oEmbed URL -> media migration class.');
     $this->source = new MigrateSourceSQL($this->query(), $fields, null, array('map_joinable' => FALSE));
     $this->map = new MigrateSQLMap($this->machineName, $key_schema, MigrateDestinationMedia::getKeySchema());

     $this->addFieldMapping('og_group_ref', 'id');
     foreach(array('field_html_title', 'field_html_code','media_title','media_description') as $field) {
       $this->addSimpleMappings(array("$field", "$field:format"));
       $this->addFieldMapping("$field:language")->defaultValue(LANGUAGE_NONE);
     }

   //  $this->addUnmigratedDestinations(array('path', 'file_replace', 'preserve_files', 'source_dir'), t('DNM'));
     $this->addSimpleMappings(array('value', 'timestamp', 'uid')); //, 'destination_file'));
  //   $this->addFieldMapping('destination_dir')->defaultValue('embed://');
  }

  public function query() {
    $query = parent::d6_db_select('spaces_overrides', 'so')
      ->condition('object_type', 'boxes')
      ->condition('value', '%os_boxes_oembed%', 'LIKE')
      ->fields('so');
    $query->join('og_uid', 'og_uid', 'og_uid.nid = so.id');
    $query->fields('og_uid', array('uid'));
    $query->condition('og_uid.is_admin', 1);
    return $query;
  }


  public function prepareRow(&$row) {
    //value, timestamp
    if (($object = @unserialize($row->value)) === FALSE) {
      parent::saveMessage(t('Could not unserialize embed box'), MigrationBase::MESSAGE_WARNING);
      return FALSE;
    }

    $row->timestamp = $object->changed;
    $selection = $object->options['selection'];
    if ($selection != 'pasted') {
      return FALSE; //lets just try url first
    }

    $row->value = 'html'; //$object->options[$selection]; //should value be an array?
   // $row->destination_file = FALSE;//$object->options[$selection];

    $row->field_html_title = $object->title;
    $row->{'field_html_title:format'} = 'plain_text';

    $row->field_html_code = $object->options[$selection];
    $row->{'field_html_code:format'} = 'filtered_html';

    $row->field_media_title = $object->title;
    $row->{'field_media_title:format'} = 'plain_text';

    $row->field_media_description = $object->description;
    $row->{'field_media_description:format'} = 'plain_text';
//file_uri_to_object($uri)

    $f = file_uri_to_object($row->value);
    $f->uid = $row->uid;
    $f->type = 'html';      //this isn't taking...
    $f->filesize = 0;
 //   $f->og_group_ref = array(LANGUAGE_NONE => array($row->id));
//     $f->field_html_code = array(LANGUAGE_NONE => array(array('value' => $row->field_html_code)));
//     $f->field_html_title = array(LANGUAGE_NONE => array(array('value' => $row->field_html_title)));
//     $f->field_media_title = array(LANGUAGE_NONE => array(array('value' => $row->field_media_title)));
//     $f->field_media_description = array(LANGUAGE_NONE => array(array('value' => $row->field_media_description)));
    $f->filemime = 'html/embed';

    $f->uri = 'embed://' . md5($row->field_html_code);
    $f = file_save($f);

    $obj = entity_metadata_wrapper('file', $f);
    $obj->field_html_code = $object->options[$selection];
    $obj->field_html_title = $object->title;
//    $obj->field_media_title = $object->title;
  //  $obj->field_media_description = $object->description;
    $obj->save();

    $row->value = $row->fid = $f->fid;


    return parent::prepareRow($row);
  }

  function prepare(&$file, $row) {
    $file;
  }
}