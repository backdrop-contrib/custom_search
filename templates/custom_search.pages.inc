<?php

/**
 * @file
 * User page callbacks for the custom_search module.
 */

/*
 * Presents links to filter the search results.
 */
function custom_search_preprocess_search_results(&$variables) {
  if ($variables['module'] == 'node') {
    
    $config = config('custom_search.settings');
    
    $variables['filter_position'] = $config->get('filter');
    // Save # of results for collapsing advanced search.
    $GLOBALS['custom_search_nb_results'] = count($variables['results']);

    // Generate the filter.
    if ($variables['filter_position'] != 'disabled') {
      // Get search words (minus type:node_type).
      $path = explode('/', $_GET['q'], 3);
      $keys = empty($_REQUEST['keys']) ? '' : $_REQUEST['keys'];
      if (count($path) == 3) {
        $keys = $path[2];
      }
      if (strpos($keys, 'type:') !== FALSE) {
        $keys = backdrop_substr($keys, 0, strpos($keys, 'type:') - 1);
      }
      // Get Custom Search authorized types.
      $searchable_node_types = array();
      $defined_types = array_filter($config->get('current_node_types'), 'custom_search_filter_array');
      $type_names = node_type_get_names();
      if (count($defined_types)) {
        foreach ($defined_types as $type) {
          $searchable_node_types[$type] = $type_names[$type];
        } 
      }
      else {
        $searchable_node_types = $type_names;
      }
      // Build menu.
      $items = array();
      $items[] = l($config->get('current_content_selector_any_text'), 'search/node/' . $keys);
      foreach ($searchable_node_types as $type => $name) {
        // Count # of results per type.
        $nbresults = 0;
        foreach ($variables['results'] as $result) {
          if ($result['node']->type == $type) {
            $nbresults++;
          }
        }
        if ($nbresults) {
          $items[] = l($name, 'search/node/' . $keys . ' type:' . $type);
        }
      }
      if (!isset($variables['filter-title'])) {
        $variables['filter-title'] = filter_xss(t($config->get('filter_label')));
      }
      if (count($items) > 2) {
        $variables['filter'] = theme('item_list', array('items' => $items, 'title' => $variables['filter-title']));
      }
    }

    // Display only the requested infos
    foreach ($variables['search_results'] as $nid => $result) {
      $infos = array();
      if ($config->get('results_info_type')) {
        $infos[] = $result['result']['type'];
      }
      if (isset($result['info_split'])) {
        foreach ($result['info_split'] as $key => $info) {
          if ($config->get('results_info_' . $key, TRUE)) {
            array_push($infos, $info);
          }
        }
      }
      $variables['search_results'][$nid]['info'] = implode(' - ', $infos);
    }

    $variables['theme_hook_suggestions'][] = 'custom_search_results';

  }
}
