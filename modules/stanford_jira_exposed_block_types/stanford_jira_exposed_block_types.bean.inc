<?php
/**
 * @file
 * stanford_jira_exposed_block_types.bean.inc
 */

/**
 * Implements hook_bean_admin_ui_types().
 */
function stanford_jira_exposed_block_types_bean_admin_ui_types() {
  $export = array();

  $bean_type = new stdClass();
  $bean_type->disabled = FALSE; /* Edit this to true to make a default bean_type disabled initially */
  $bean_type->api_version = 5;
  $bean_type->name = 'stanford_jira';
  $bean_type->label = 'Stanford Jira';
  $bean_type->options = '';
  $bean_type->description = 'Allows the content author to select from a number of configuration options in order to display ticket information from Jira.';
  $export['stanford_jira'] = $bean_type;

  return $export;
}
