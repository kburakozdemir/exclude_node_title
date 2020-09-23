<?php

/**
 * @file
 * Exclude Node Title Administrative Interface.
 */

/**
 * Admin configuration form.
 */
function exclude_node_title_admin_settings() {
  $form['exclude_node_title_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove node title from search pages'),
    '#description' => t('Enable title exclusion in search pages. The Search module must be !enabled.', array('!enabled' => l(t('enabled'), 'admin/modules/list'))),
    '#default_value' => variable_get('exclude_node_title_search', 0),
    '#disabled' => !module_exists('search'),
  );

  $form['exclude_node_title_translation_sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Syncronize content translations'),
    '#description' => t('Enable title exclusion in translated versions of nodes. The Content Translation module must be !enabled.', array('!enabled' => l(t('enabled'), 'admin/modules/list'))),
    '#default_value' => variable_get('exclude_node_title_translation_sync', TRUE),
    '#disabled' => !module_exists('translation'),
  );

  $form['exclude_node_title_content_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Exclude the title by content type.'),
    '#description' => t('Select the content types for which the node title will be excluded.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['exclude_node_title_hide_using_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide node title using CSS'),
    '#description' => t('Select if you wish to use CSS to hide the node title. This will make your page accessible for both screen-readers and search engines as they utilize heading tags. You will need to place this variable "$exclude_node_title_class" in the page title class attribute, in both your page.tpl.php and node.tpl.php template for this feature to take effect.'),
    '#default_value' => variable_get('exclude_node_title_hide_using_css', 0),
    '#weight' => -2,
  );

  $form['exclude_node_title_themes_supporting_css'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array_keys(list_themes())),
    '#description' => t('Select the themes in which the page.tpl.php and node.tpl.php templates contain the variable "$exclude_node_title_class". You can find an example <a href="@link">here</a>.', array('@link' => 'https://www.drupal.org/node/1057044#comment-4080172')),
    '#multiple' => TRUE,
    '#default_value' => variable_get('exclude_node_title_themes_supporting_css', array()),
    '#states' => array(
      'visible' => array(
        'input[name="exclude_node_title_hide_using_css"]' => array('checked' => TRUE),
      ),
    ),
    '#weight' => -1,
  );

  foreach (node_type_get_names() as $node_type => $node_type_label) {
    drupal_add_js(array('exclude_node_title' => array('content_types' => array($node_type => $node_type_label))), 'setting');
    $node_type_options = array(
      'none' => t('None'),
      'all' => t('All nodes'),
      'user' => t('User defined nodes'),
    );

    $form['exclude_node_title_content_type']['exclude_node_title_content_type_value_' . $node_type] = array(
      '#type' => 'select',
      '#title' => $node_type_label,
      '#default_value' => variable_get('exclude_node_title_content_type_value_' . $node_type, 'none'),
      '#options' => $node_type_options,
    );

    $entity_info = entity_get_info('node');
    $view_modes = $entity_info['view modes'];
    $modes = array();
    foreach ($view_modes as $view_mode_name => $view_mode_info) {
      $modes[$view_mode_name] = $view_mode_info['label'];
    }
    $modes += array('nodeform' => t('Node form'));

    switch ($form['exclude_node_title_content_type']['exclude_node_title_content_type_value_' . $node_type]['#default_value']) {
      case 'all':
        $title = t('Exclude title from all nodes in the following view modes:');
        break;

      case 'user defined':
        $title = t('Exclude title from user defined nodes in the following view modes:');
        break;

      default:
        $title = t('Exclude from:');
    }

    $form['exclude_node_title_content_type']['exclude_node_title_content_type_modes_' . $node_type] = array(
      '#type' => 'checkboxes',
      '#title' => $title,
      '#default_value' => variable_get('exclude_node_title_content_type_modes_' . $node_type, array()),
      '#options' => $modes,
      '#states' => array(
        // Hide the modes when the content type value is <none>.
        'invisible' => array(
          'select[name="exclude_node_title_content_type_value_' . $node_type . '"]' => array('value' => 'none'),
        ),
      ),
    );
  }

  drupal_add_js(drupal_get_path('module', 'exclude_node_title') . '/exclude_node_title.js');

  return system_settings_form($form);
}
