<?php

/**
 * @file
 * Views definitions.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_oai_pmh_custom_code_views_data_alter(&$data) {
  $data['node']['author_name'] = array(
    'title' => t('Author Name'),
    'help' => t('The Author Name'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_name',
    ),
  );

  $data['node']['author_lastname'] = array(
    'title' => t('Author Lastname'),
    'help' => t('The Author Lastname'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_lastname',
    ),
  );

  $data['node']['author_cleautorite'] = array(
    'title' => t('Author Cleautorite'),
    'help' => t('The Cleautorite'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_cleautorite',
    ),
  );

}
