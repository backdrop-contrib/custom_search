<?php

/**
 * @file
 * Admin settings for custom search taxonomy
 */

function custom_search_taxonomy_admin() {
  $vocabularies = taxonomy_get_vocabularies();
  if (count($vocabularies)) {
    $form = _custom_search_taxonomy_admin_form($vocabularies);
    return system_settings_form($form);
  }
}
