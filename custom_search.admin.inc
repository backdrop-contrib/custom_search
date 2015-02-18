<?php

/**
 * @file
 * Admin settings for custom search.
 */

function custom_search_admin_form() {

  $config = config('custom_search.settings');

  $form['target'] = array(
    '#type'     => 'select',
    '#title'    => t('Results page opens in'),
    '#options'  => array(
      '_self'   => t('the same window'),
      '_blank'  => t('a new window'),
    ),
    '#default_value'  => $config->get('target'),
  );

  $form['search_form'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Search form'),
  );
  $form['search_form']['results_search'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Display basic search'),
    '#default_value'  => $config->get('results_search'),
  );
  $form['search_form']['results_advanced_search'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Display advanced search'),
    '#default_value'  => $config->get('results_advanced_search'),
  );
  $form['search_form']['advanced'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Advanced search'),
    '#states' => array(
      'visible' => array(
        ':input[name="results_advanced_search"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['search_form']['advanced']['results_advanced_search_collapsible'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Collapsible'),
    '#default_value'  => $config->get('results_advanced_search_collapsible'),
  );
  $form['search_form']['advanced']['results_advanced_search_collapsed'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Collapsed'),
    '#default_value'  => $config->get('results_advanced_search_collapsed'),
  );
  $form['search_form']['advanced']['criteria'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Criteria'),
    '#description'  => t('Select the criteria to display on the advanced search form.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $form['search_form']['advanced']['criteria']['advanced_or_display'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Or'),
    '#default_value'  => $config->get('advanced_or_display'),
  );
  $form['search_form']['advanced']['criteria']['advanced_phrase_display'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Phrase'),
    '#default_value'  => $config->get('advanced_phrase_display'),
  );
  $form['search_form']['advanced']['criteria']['advanced_negative_display'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Negative'),
    '#default_value'  => $config->get('advanced_negative_display'),
  );
  $form['search_form']['advanced']['types'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Content types'),
    '#description'  => t('Select the content types to display on the advanced search form.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => TRUE,
  );
  $options = node_type_get_names();
  foreach ($options as $machine_name => $name) {
    $form['search_form']['advanced']['types']['advanced_type_' . $machine_name . '_display'] = array(
      '#type'           => 'checkbox',
      '#title'          => $name,
      '#default_value'  => $config->get('advanced_type_' . $machine_name . '_display', TRUE),
    );
  }

  // Languages.
  $language_options = array();
  foreach (language_list('language') as $langcode => $language) {
    if ($language->enabled) {
      $language_options[$langcode] = $language->name;
    }
  }
  if (count($language_options) > 1) {
    $form['search_form']['advanced']['languages'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('Languages'),
      '#description'  => t('Select the languages to display on the advanced search form.'),
      '#collapsible'  => TRUE,
      '#collapsed'    => TRUE,
    );
    foreach ($language_options as $langcode => $name) {
      $form['search_form']['advanced']['languages']['advanced_language_' . $langcode . '_display'] = array(
        '#type'           => 'checkbox',
        '#title'          => $name,
        '#default_value'  => $config->get('advanced_language_' . $langcode . '_display', TRUE),
      );
    }
  }

  $form['results'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Results'),
    '#description'  => t('Select data to display below each result.'),
  );
  $form['results']['results_info_type'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Content type'),
    '#default_value'  => $config->get('results_info_type', TRUE),
  );
  $form['results']['results_info_user'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('User'),
    '#default_value'  => $config->get('results_info_user', TRUE),
  );
  $form['results']['results_info_date'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Date'),
    '#default_value'  => $config->get('results_info_date', TRUE),
  );
  if (module_exists('comment')) {
    $form['results']['results_info_comment'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Comments'),
      '#default_value'  => $config->get('results_info_comment', TRUE),
    );
  }

  $form['filter'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Filter'),
    '#description'  => t('Add links to filter the results by content type.'),
  );
  $form['filter']['filter'] = array(
    '#type'           => 'select',
    '#title'          => t('Position'),
    '#options'        => array(
      'disabled'  => t('Disabled'),
      'above'     => t('Above results'),
      'below'     => t('Below results'),
    ),
    '#default_value'  => $config->get('filter', 'disabled'),
  );
  $form['filter']['filter_label'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Label text'),
    '#default_value'  => $config->get('filter_label'),
    '#description'    => t('Enter the label text for the list. The default value is "!default".', array('!default' => CUSTOM_SEARCH_FILTER_LABEL_DEFAULT)),
    '#states' => array(
      'invisible' => array(
        ':input[name="filter"]' => array('value' => 'disabled'),
      ),
    ),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for custom_search_admin_form_submit().
 */
function custom_search_admin_form_submit($form, &$form_state) {
  $config = config('custom_search.settings');

  $config->set('target', $form_state['values']['target']);
  $config->set('results_search', $form_state['values']['results_search']);
  $config->set('results_advanced_search', $form_state['values']['results_advanced_search']);
  $config->set('results_advanced_search_collapsible', $form_state['values']['results_advanced_search_collapsible']);
  $config->set('results_advanced_search_collapsed', $form_state['values']['results_advanced_search_collapsed']);
  $config->set('advanced_or_display', $form_state['values']['advanced_or_display']);
  $config->set('advanced_phrase_display', $form_state['values']['advanced_phrase_display']);
  $config->set('advanced_negative_display', $form_state['values']['advanced_negative_display']);
  $types = node_type_get_names();
  foreach ($types as $machine_name => $name) {
    $config->set('advanced_type_' . $machine_name . '_display', $form_state['values']['advanced_type_' . $machine_name . '_display']);
  }
  foreach (language_list('language') as $langcode => $language) {
    $config->set('advanced_language_' . $langcode . '_display', $form_state['values']['advanced_language_' . $langcode . '_display']);
  }
  $config->set('results_info_type', $form_state['values']['results_info_type']);
  $config->set('results_info_user', $form_state['values']['results_info_user']);
  $config->set('results_info_date', $form_state['values']['results_info_date']);
  if (module_exists('comment')) {
    $config->set('results_info_comment', $form_state['values']['results_info_comment']);
  }
  $config->set('filter', $form_state['values']['filter']);
  $config->set('filter_label', $form_state['values']['filter_label']);

  $config->save();

  backdrop_set_message(t('The configuration options have been saved.'));
}