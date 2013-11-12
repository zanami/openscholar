<?php 

/**
 * @file node_event.inc
 * 
 * event plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_event'),
  'entity_type' => array('node'),
  'bundle' => array('event'),
);

class sv_list_node_event {
  
  /**
   * @function sort_alter
   */
//   public function sort_alter($options, $efq) {  
//     if ($options['sort_by'] == 'sort_event') {
      //filters new events, and handles repeats.
//       $query = db_select('field_data_field_date', 'fd')
//         ->fields('fd', array('entity_id', 'delta'))
//         ->where('field_date_value > NOW()')
//         ->orderBy('field_date_value');
//       $query->join('node', 'n', 'n.vid = fd.revision_id');
//       $query->condition('n.status', 1);
//       $entity_field = "fd.entity_id";
//     }
//   }
  public function query_alter($options, $efq) {
    //get the entity delta.  I don't know why.
    //         ->fields('fd', array('entity_id', 'delta'))
  }

  /**
   * @function sort_event
   * 
   * Custom sort for events.  Uses event date, but only for
   * upcoming events that haven't taken place yet.
   */
  public function sort_event($options, $efq) {
    $efq->fieldOrderBy('field_date', 'value');
    $efq->fieldCondition('field_date', 'value', date('Y-m-d'), '>=');
  }
}
