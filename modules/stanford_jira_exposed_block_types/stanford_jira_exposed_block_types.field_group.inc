<?php
/**
 * @file
 * stanford_jira_exposed_block_types.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function stanford_jira_exposed_block_types_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_jira_admins|bean|stanford_jira|form';
  $field_group->group_name = 'group_jira_admins';
  $field_group->entity_type = 'bean';
  $field_group->bundle = 'stanford_jira';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Administrative',
    'weight' => '4',
    'children' => array(
      0 => 'revision',
      1 => 'view_mode',
      2 => 'redirect',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-jira-admins field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_jira_admins|bean|stanford_jira|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_jira_content|bean|stanford_jira|form';
  $field_group->group_name = 'group_jira_content';
  $field_group->entity_type = 'bean';
  $field_group->bundle = 'stanford_jira';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Content & Display',
    'weight' => '2',
    'children' => array(
      0 => 'field_jira_body_below',
      1 => 'field_jira_body_above',
      2 => 'title',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-jira-content field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_jira_content|bean|stanford_jira|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_jira|bean|stanford_jira|form';
  $field_group->group_name = 'group_jira';
  $field_group->entity_type = 'bean';
  $field_group->bundle = 'stanford_jira';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Jira Settings',
    'weight' => '3',
    'children' => array(
      0 => 'field_jira_jql',
      1 => 'field_jira_layout',
      2 => 'field_jira_field_storage',
      3 => 'field_jira_field_options',
      4 => 'field_jira_fields',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-jira field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_jira|bean|stanford_jira|form'] = $field_group;

  return $export;
}
