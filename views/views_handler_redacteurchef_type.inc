<?php

/**
 * @file
 * Definition of views_handler_redacteurchef_type.
 */

/**
 * Show the redacteurchef type.
 */
class views_handler_redacteurchef_type extends views_handler_field {

  /**
   * Does the field supports multiple field values.
   *
   * @var bool
   */
  public $multiple;

  /**
   * Does the rendered fields get limited.
   *
   * @var bool
   */
  public $limit_values;

  /**
   * {@inheritdoc}
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);

    $this->multiple = TRUE;
    $this->limit_values = FALSE;
  }

  /**
   * Add some required fields needed on render().
   */
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array(
      'table' => 'node',
      'field' => 'nid',
    );
  }

  /**
   * {@inheritdoc}
   *
   * Perform any database or cache data retrieval here. In this example there is
   * none.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * {@inheritdoc}
   *
   * Modify any end user views settings here. Debug $options to view the field
   * settings you can change.
   */
  function option_definition() {
    $options = parent::option_definition();

    // Options used for multiple value fields.
    $options['group_rows'] = array(
      'default' => TRUE,
      'bool' => TRUE,
    );
    $options['delta_limit'] = array(
      'default' => 'all',
    );
    $options['delta_offset'] = array(
      'default' => 0,
    );
    $options['delta_reversed'] = array(
      'default' => FALSE,
      'bool' => TRUE,
    );
    $options['delta_first_last'] = array(
      'default' => FALSE,
      'bool' => TRUE,
    );
    $options['delta_random'] = array(
      'default' => FALSE,
      'bool' => TRUE,
    );

    $options['multi_type'] = array(
      'default' => 'separator',
    );
    $options['separator'] = array(
      'default' => ',,, ',
    );

    $options['hide_empty'] = array(
      'default' => TRUE,
      'bool' => TRUE,
    );

    return $options;
  }

  /**
   * {@inheritdoc}
   *
   * Make changes to the field settings form seen by the end user when adding
   * your field.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $this->multiple_options_form($form, $form_state);
  }

  /**
   * Provide options for multiple value fields.
   */
  public function multiple_options_form(&$form, &$form_state) {
    $field = $this->field_info;

    $form['multiple_field_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Multiple field settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 5,
    );

    $form['group_rows'] = array(
      '#title' => t('Display all values in the same row'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['group_rows'],
      '#description' => t('If checked, multiple values for this field will be shown in the same row. If not checked, each value in this field will create a new row. If using group by, please make sure to group by "Entity ID" for this setting to have any effect.'),
      '#fieldset' => 'multiple_field_settings',
    );

    // Make the string translatable by keeping it as a whole rather than
    // translating prefix and suffix separately.
    list($prefix, $suffix) = explode('@count', t('Display @count value(s)'));

    $type = 'textfield';
    $options = NULL;
    $size = 5;

    $form['multi_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#options' => array(
        'ul' => t('Unordered list'),
        'ol' => t('Ordered list'),
        'separator' => t('Simple separator'),
      ),
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#default_value' => $this->options['multi_type'],
      '#fieldset' => 'multiple_field_settings',
    );

    $form['separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => $this->options['separator'],
      '#dependency' => array(
        'radio:options[multi_type]' => array('separator'),
        'edit-options-group-rows' => array(TRUE),
      ),
      '#dependency_count' => 2,
      '#fieldset' => 'multiple_field_settings',
    );

    $form['delta_limit'] = array(
      '#type' => $type,
      '#size' => $size,
      '#field_prefix' => $prefix,
      '#field_suffix' => $suffix,
      '#options' => $options,
      '#default_value' => $this->options['delta_limit'],
      '#prefix' => '<div class="container-inline">',
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#fieldset' => 'multiple_field_settings',
    );

    list($prefix, $suffix) = explode('@count', t('starting from @count'));
    $form['delta_offset'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#field_prefix' => $prefix,
      '#field_suffix' => $suffix,
      '#default_value' => $this->options['delta_offset'],
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#description' => t('(first item is 0)'),
      '#fieldset' => 'multiple_field_settings',
    );
    $form['delta_reversed'] = array(
      '#title' => t('Reversed'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['delta_reversed'],
      '#suffix' => $suffix,
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#description' => t('(start from last values)'),
      '#fieldset' => 'multiple_field_settings',
    );
    $form['delta_first_last'] = array(
      '#title' => t('First and last only'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['delta_first_last'],
      '#suffix' => $suffix,
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#fieldset' => 'multiple_field_settings',
    );
    $form['delta_random'] = array(
      '#title' => t('Random order'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['delta_random'],
      '#suffix' => '</div>',
      '#dependency' => array('edit-options-group-rows' => array(TRUE)),
      '#fieldset' => 'multiple_field_settings',
    );
  }

  /**
   * Render callback handler.
   *
   * Return the markup that will appear in the rendered field.
   */
  function render($values) {
    // Getting the nid.
    $nid = $this->get_value($values, 'nid');
    // Loading the node.
    $node = node_load($nid);
    $values = array();

    if (in_array($node->type, array('article_publication', 'entretien'))) {
      // Getting the dossier id;
      $dossier_id = $node->field_dossier_referent[LANGUAGE_NONE][0]['target_id'];
      // Loading the dossier
      $dossier = node_load($dossier_id);

      foreach ($dossier->field_type_responsable_dossier[LANGUAGE_NONE] as $directeur) {
        // Getting the field_collection.
        $field_collection = field_collection_item_load($directeur['value'], $reset = FALSE);
        // Getting the directeur id.
        $type_id = $field_collection->field_type_direction[LANGUAGE_NONE][0]['tid']; dpm($field_collection);
        // Getting the type taxonomy.
        $type_direction_taxonomy = taxonomy_term_load($type_id);
        $values[] = $type_direction_taxonomy->name;
      }

      // Getting the names list.
      $values = implode(filter_xss_admin($this->options['separator']), $values);
    }

    return $values;
  }

}
