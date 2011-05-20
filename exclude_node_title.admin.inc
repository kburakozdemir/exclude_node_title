<?php
/**
 * @file
 * Exclude Node Title Administrative Interface
 *
 * @author Gabriel Ungureanu
 * gabriel.ungreanu@ag-prime.com
 */

/**
 * Admin configuration form
 */
function exclude_node_title_admin_settings() {
  $form['exclude_node_title_remove_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove node title from teasers too'),
    '#description' => t('Select if you wish to remove title from node teasers.'),
    '#default_value' => variable_get('exclude_node_title_remove_title', 0),
  );

  $form['exclude_node_title_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove node title from search pages'),
    '#description' => t('Select if you wish to remove title from search pages.'),
    '#default_value' => variable_get('exclude_node_title_search', 0),
  );

  $form['exclude_node_title_content_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Exclude title by content-types'),
    '#description' => t('Define title excluding settings for each content type.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $node_types = _node_types_build()->names;
  foreach ($node_types as $node_type => $node_type_label) {
    $form['exclude_node_title_content_type']['exclude_node_title_content_type_value_' . $node_type] = array(
      '#type' => 'select',
      '#title' => $node_type_label,
      '#default_value' => variable_get('exclude_node_title_content_type_value_' . $node_type, 'none'),
      '#options' => array('none' => t('None'), 'all' => t('All nodes'), 'user' => t('User defined nodes')),
    );
  }

  $form['#submit'][] = 'exclude_node_title_admin_submit';
  return system_settings_form($form);
}
/**
 * Submit function for hook_admin_settings form
 * It processes list of nodes in exception list and saves them as array instead of plain text.
 * Comes before saving actual values in the databse.
 */
function exclude_node_title_admin_submit($form, &$form_state) {
  // process node id exceptions list
  $exception_ids = $form_state['values']['exclude_node_title_nid_list'];
  $exception_ids = explode("\n", $exception_ids);
  $exception_ids = array_map('trim', $exception_ids);
  $form_state['values']['exclude_node_title_nid_list'] = $exception_ids;
}
