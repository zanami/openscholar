<?php 

class ClassNodeOSMigration extends AbstractNodeOSMigration {
  public function __construct() {
    
    // Let AbstractNode handle the source, dest, and map.
    parent::__construct('class'); //current types are vsite, article, and page.  lets start with one of those instead?
    $this->description = t('OS Class Nodes');    
    
    
    $fields = array(
      'url' =>          array('source_field' => 'field_class_link_url'),
      'title' =>        array('source_field' => 'field_class_link_title'),
      'attributes' =>   array('source_field' => 'field_class_link_attributes'), //doublecheck that these don't need fudging
    );
    
    
    // $arguments = MigrateLinkFieldHandler::arguments($fields);
    //pass info from fields to arguments[] in field_link's prepare()
    $this->addFieldMapping('field_link', 'field_link')->arguments($fields);

    $this->addUnmigratedSources(array('field_class_link_url', 'field_class_link_title', 'field_class_link_attributes'), t('DNM'));
    
    /************************************************************************
     *  Field mappings tell Migrate which d7 column to deposit d6 data into *
     ************************************************************************/
    
  }
  
  //also include content type's data
  public function query($bundle) {
    $query = parent::query($bundle);
    $query->join('content_type_class', 'ct', 'n.nid = ct.nid');
    $query->fields('ct', array('field_class_link_url', 'field_class_link_title', 'field_class_link_attributes'));
    return $query;
  }
  
  
  public function sourceFields() {
    $fields = parent::sourceFields();
    $fields['field_link'] = t('Linked URL');
    return $fields;
  }
  
  public function prepareRow($current_row) {
    parent::prepareRow($current_row); //why no pass by ref?

    //field_link
    if (strlen($current_row->field_class_link_url)) {

      if (strlen($current_row->field_class_link_attributes)) {
        $current_row->field_class_link_attributes = unserialize($current_row->field_class_link_attributes);
      }
      //$current_row->field_class_link_attributes = ($current_row->field_class_clink_attributes) ? 
        //unserialize($current_row->field_class_link_attributes) : array();
      $current_row->field_link[] = $current_row->field_class_link_url;
    }
  }
}