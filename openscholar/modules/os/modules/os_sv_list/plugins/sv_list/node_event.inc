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
    $efq->addTag('event');
  }
  
  /**
   * @function entities_alter
   * 
   * Events have been loaded, but repeated events are grouped under a single node.
   * Split them into several nodes, then re-sort and slice them.
   */
  public function entities_alter($options, &$entities) {
    if ($options['content_type'] != 'event' || $options['sort_by'] != 'sort_event') {
      return;
    }
    
//     $query = db_select('field_data_field_date', 'fd')
//       ->fields('fd', array('entity_id', 'delta')) //@TODO date sehnanigans
//       ->condition('entity_id', array_keys($entities), 'IN')
//       ->where('field_date_value > NOW()')
//       ->orderBy('field_date_value');
//     $query->join('node', 'n', 'n.vid = fd.revision_id');
//     $query->condition('n.status', 1);
//     $result = $query->execute();
    
//     while ($repeat = $result->fetchObject()) {
//       $repeat;
//     }
    $exploded = array();
    foreach ($entities as $entity) {
      foreach ($entity->field_date[LANGUAGE_NONE] as $date) {
        $new = clone $entity;
        $new->field_date[LANGUAGE_NONE][0] = $date;
        $exploded[] = $new;
        
      }
    }
    
    $entities = $exploded;
    //how to sort?
  }
}
