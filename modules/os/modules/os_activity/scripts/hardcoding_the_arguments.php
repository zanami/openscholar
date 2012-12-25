<?php

/**
 * @file
 * Drush script for replacing all of the arguments from tokens to hard coded
 * string. This will increase the performance of the activity stream view.
 */

// Collect drush arguments.
$batch = drush_get_option('batch', 250);
$memory_limit = drush_get_option('memory_limit', 500);
$id = drush_get_option('id', 1);

// Count how much messages we have.
$query = new EntityFieldQuery();
$max = $query
  ->entityCondition('entity_type', 'message')
  ->propertyCondition('mid', $id, '>=')
  ->count()
  ->execute();

$i = 0;
while ($max > $i) {

  // Collect the messages in batches.
  $query = new EntityFieldQuery();
  $result = $query
    ->entityCondition('entity_type', 'message')
    ->propertyCondition('mid', $id, '>=')
    ->propertyOrderBy('mid')
    ->range($i, $i + $batch)
    ->execute();

  if (empty($result['message'])) {
    return;
  }

  foreach (array_keys($result['message']) as $mid) {
    // Load the message.
    $message = message_load($mid);

    foreach (array('message:field-node-reference:url','message:field-node-reference:title') as $token) {
      // Check that the token is not hard coded.
      if (isset($message->arguments['@{' . $token . '}'])) {
        continue;
      }

      // Creating a hard coded value for the toekn.
      $token_options = message_get_property_values($message, 'data', 'token options');
      $context = array('message' => $message);

      $message->arguments['@{' . $token . '}'] = token_replace('[' . $token . ']', $context, $token_options);

      // Saving the message and the dispaly message for the user.
      message_save($message);

      $param = array(
        '@mid' => $mid,
        '@toekn' => $token,
      );
      drush_log(dt('Hard coded value were created for the token @toekn in the message @mid', $param), 'success');

      // The script taking to much memory. Stop it and display message.
      if (round(memory_get_usage()/1048576) >= $memory_limit) {
        return drush_set_error('OS_ACTIVITY OUT_OF_MEMORY', dt('Stopped before out of memory. Last message ID was @mid', array('@mid' => $mid)));
      }

      $i++;
    }
  }
}
