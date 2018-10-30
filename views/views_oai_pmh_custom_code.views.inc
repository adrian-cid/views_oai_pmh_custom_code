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
    'help' => t('The Author Cleautorite'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_cleautorite',
    ),
  );

  $data['node']['author_id'] = array(
    'title' => t('Author Id'),
    'help' => t('The Author ID'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_id',
    ),
  );

  $data['node']['redacteurchef_name'] = array(
    'title' => t('Redacteur chef Name'),
    'help' => t('The Redacteur Chef Name'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_redacteurchef_name',
    ),
  );

  $data['node']['redacteurchef_lastname'] = array(
    'title' => t('Redacteur chef Lastname'),
    'help' => t('The Redacteur Chef Lastname'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_redacteurchef_lastname',
    ),
  );

  $data['node']['redacteurchef_genre'] = array(
    'title' => t('Redacteur chef sexe'),
    'help' => t('The Redacteur Chef Sexe'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_redacteurchef_genre',
    ),
  );

  $data['node']['redacteurchef_type'] = array(
    'title' => t('Redacteur chef type'),
    'help' => t('The Redacteur Chef Type'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_redacteurchef_type',
    ),
  );

  $data['node']['author_contribution'] = array(
    'title' => t('Author Contribution'),
    'help' => t('The Author Contribution'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_contribution',
    ),
  );

  $data['node']['author_typecontrib'] = array(
    'title' => t('Author Type Contribution'),
    'help' => t('The Author Type Contribution'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_typecontrib',
    ),
  );

  $data['node']['author_notebio'] = array(
    'title' => t('Author Notebio'),
    'help' => t('The Author Notebio'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_author_notebio',
    ),
  );

  $data['node']['refbiblio'] = array(
    'title' => t('Bibliographic Reference'),
    'help' => t('The Bibliographic Reference'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_refbiblio',
    ),
  );

}
