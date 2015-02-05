<?php

/**
 * @file
 * Admin settings for custom search
 */

function custom_search_blocks_admin() {
  $form['custom_search_blocks_number'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Number of blocks'),
    '#size'           => 2,
    '#default_value'  => variable_get('custom_search_blocks_number', 1),
  );
  return system_settings_form($form);
}
