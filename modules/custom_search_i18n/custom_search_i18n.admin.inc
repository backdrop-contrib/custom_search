<?php

/**
 * @file
 * Admin settings for custom search taxonomy
 */

function custom_search_i18n_admin() {
  $form = _custom_search_i18n_admin_form();
  return system_settings_form($form);
}

/**
 * Content admin form.
 */
function _custom_search_i18n_admin_form() {
  $form['custom_search_i18n_search_language'] = array(
    '#type'           => 'select',
    '#title'          => t('Search content from'),
    '#options'        => array(
      'all'             => t('any language'),
      'current'         => t('current language'),
      'current_neutral' => t('current language and language neutral'),
    ),
    '#description'    => t('Choose which language will be used to search content.'),
    '#default_value'  => variable_get('custom_search_i18n_search_language', 'all'),
  );

  return $form;
}
