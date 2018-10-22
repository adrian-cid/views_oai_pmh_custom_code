<?php

/**
 * @file
 * Definition of views_handler_redacteurchef_genre.
 */

/**
 * Show the redacteurchef genre.
 */
class views_handler_redacteurchef_genre extends views_handler_field {

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
    $this->additional_fields['nid'] = [
      'table' => 'node',
      'field' => 'nid',
    ];
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
    $options['group_rows'] = [
      'default' => TRUE,
      'bool' => TRUE,
    ];
    $options['delta_limit'] = [
      'default' => 'all',
    ];
    $options['delta_offset'] = [
      'default' => 0,
    ];
    $options['delta_reversed'] = [
      'default' => FALSE,
      'bool' => TRUE,
    ];
    $options['delta_first_last'] = [
      'default' => FALSE,
      'bool' => TRUE,
    ];
    $options['delta_random'] = [
      'default' => FALSE,
      'bool' => TRUE,
    ];

    $options['multi_type'] = [
      'default' => 'separator',
    ];
    $options['separator'] = [
      'default' => ',,, ',
    ];

    $options['hide_empty'] = [
      'default' => TRUE,
      'bool' => TRUE,
    ];

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

    $form['multiple_field_settings'] = [
      '#type' => 'fieldset',
      '#title' => t('Multiple field settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 5,
    ];

    $form['group_rows'] = [
      '#title' => t('Display all values in the same row'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['group_rows'],
      '#description' => t('If checked, multiple values for this field will be shown in the same row. If not checked, each value in this field will create a new row. If using group by, please make sure to group by "Entity ID" for this setting to have any effect.'),
      '#fieldset' => 'multiple_field_settings',
    ];

    // Make the string translatable by keeping it as a whole rather than
    // translating prefix and suffix separately.
    list($prefix, $suffix) = explode('@count', t('Display @count value(s)'));

    $type = 'textfield';
    $options = NULL;
    $size = 5;

    $form['multi_type'] = [
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#options' => [
        'ul' => t('Unordered list'),
        'ol' => t('Ordered list'),
        'separator' => t('Simple separator'),
      ],
      '#dependency' => ['edit-options-group-rows' => [TRUE]],
      '#default_value' => $this->options['multi_type'],
      '#fieldset' => 'multiple_field_settings',
    ];

    $form['separator'] = [
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => $this->options['separator'],
      '#dependency' => [
        'radio:options[multi_type]' => ['separator'],
        'edit-options-group-rows' => [TRUE],
      ],
      '#dependency_count' => 2,
      '#fieldset' => 'multiple_field_settings',
    ];

    $form['delta_limit'] = [
      '#type' => $type,
      '#size' => $size,
      '#field_prefix' => $prefix,
      '#field_suffix' => $suffix,
      '#options' => $options,
      '#default_value' => $this->options['delta_limit'],
      '#prefix' => '<div class="container-inline">',
      '#dependency' => ['edit-options-group-rows' => [TRUE]],
      '#fieldset' => 'multiple_field_settings',
    ];

    list($prefix, $suffix) = explode('@count', t('starting from @count'));
    $form['delta_offset'] = [
      '#type' => 'textfield',
      '#size' => 5,
      '#field_prefix' => $prefix,
      '#field_suffix' => $suffix,
      '#default_value' => $this->options['delta_offset'],
      '#dependency' => ['edit-options-group-rows' => [TRUE]],
      '#description' => t('(first item is 0)'),
      '#fieldset' => 'multiple_field_settings',
    ];
    $form['delta_reversed'] = [
      '#title' => t('Reversed'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['delta_reversed'],
      '#suffix' => $suffix,
      '#dependency' => ['edit-options-group-rows' => [TRUE]],
      '#description' => t('(start from last values)'),
      '#fieldset' => 'multiple_field_settings',
    ];
    $form['delta_first_last'] = [
      '#title' => t('First and last only'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['delta_first_last'],
      '#suffix' => $suffix,
      '#dependency' => ['edit-options-group-rows' => [TRUE]],
      '#fieldset' => 'multiple_field_settings',
    ];
    $form['delta_random'] = [
      '#title' => t('Random order'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['delta_random'],
      '#suffix' => '</div>',
      '#dependency' => ['edit-options-group-rows' => [TRUE]],
      '#fieldset' => 'multiple_field_settings',
    ];
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

    $values = [];

    if ($node->type == 'article_publication') {
      // Getting the dossier id;
      $dossier_id = $node->field_dossier_referent[LANGUAGE_NONE][0]['target_id'];
      // Loading the dossier
      $dossier = node_load($dossier_id);

      foreach ($dossier->field_type_responsable_dossier[LANGUAGE_NONE] as $directeur) {
        // Getting the field_collection.
        $field_collection = field_collection_item_load($directeur['value'], $reset = FALSE);
        // Getting the directeur id
        $directeur_id = $field_collection->field_responsable_dossier[LANGUAGE_NONE][0]['target_id'];
        // Getting the directeur node.
        $directeur_node = node_load($directeur_id);

        $genre_id = $directeur_node->field_genre[LANGUAGE_NONE][0]['tid'];
        // Getting the genre taxonomy.
        $genre_taxonomy = taxonomy_term_load($genre_id);
        $values[] = $genre_taxonomy->name;
      }
      // Getting the names list.
      $values = implode(filter_xss_admin($this->options['separator']), $values);
    }

    return $values;
  }

}
