<?php

/**
 * Implements hook_views_pre_view().
 */
function vsite_access_views_pre_view(&$view, &$display_id, &$args){

	$space = spaces_get_space();
	
	//If we are not in a vsite, add our filter, inside a vsite we can restrict access without filters overhead
	if (false || !is_object($space)) {
		
		//After we have determined that we are not in an OG space lets make
		//sure we add our filters
		$group_filter = $view->get_item($display_id, 'filter', 'group_access_value');

		//If the group filter is already present there is no need to do anything
		if (!$group_filter) {

			$table = $view->base_table;

			if (in_array($table, array(
					'node'
			))) {
				if (! $view->get_item($display_id, 'relationship', 'og_membership_rel')) {
					//lets add the Relationship to the og table
					$view->add_item($display_id, 'relationship', $table, 'og_membership_rel');
				}
				
				//Lets add the relationship to the group node
				if (! $view->get_item($display_id, 'relationship', 'og_membership_related_node')) {
					//lets add the Relationship to the og table
					$view->add_item($display_id, 'relationship', 'og_membership', 'og_membership_related_node');
				}
				
				$group_filter = array(
						'id' => 'group_access_value',
						'table' => 'field_data_group_access',
						'field' => 'group_access_value',
						'group_type' => 'group',
						'operator' => 'in',
						'group' => '1',
						'exposed' => false,
						'relationship' => 'node_og_membership',
						'value' => array(1 => 1),
				);
				
				//Lets add the filter for group access field
				$view->set_item($display_id, 'filter', 'group_access_value', $group_filter);

			}

			//$view->set_item_option($display_id, 'filter', 'vsssssite_access', 'relationship', 'og_membership_rel);
		}
	}
}

/**
 * Override the node access plugin for views
 *
 * @param array $plugins
 */
function vsite_access_views_data_alter(&$data){

	// ----------------------------------------------------------------------
	// Vsite access
	$data['node_access']['nid']['filter']['handler'] = 'views_handler_filter_vsite_access';
	return;

	if(isset($data['og_membership']['og_membership_related_node'])){
		$data['og_membership']['og_membership_related_node']['etid']['filter'] = array(
				'handler' => 'views_handler_filter_vsite_access',
		);
	}

	return;

	// 	// Define the base group of this table. Fields that don't
	// 	// have a group defined will go into this field by default.
	// 	$data['node_access']['table']['group']  = t('Content access');

	// 	// For other base tables, explain how we join
	// 	$data['node_access']['table']['join'] = array(
	// 			// Directly links to node table.
	// 			'node' => array(
	// 					'left_field' => 'nid',
	// 					'field' => 'nid',
	// 			),
	// 	);
	// 	// nid field
	// 	$data['node_access']['nid'] = array(
	// 			'title' => t('Access'),
	// 			'help' => t('Filter by access.'),
	// 			'filter' => array(
	// 					'handler' => 'views_handler_filter_node_access',
	// 					'help' => t('Filter for content by view access. <strong>Not necessary if you are using node as your base table.</strong>'),
	// 			),
	// 	);

	return $data;
}