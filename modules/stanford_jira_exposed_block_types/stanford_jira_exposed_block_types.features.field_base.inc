<?php
/**
 * @file
 * stanford_jira_exposed_block_types.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function stanford_jira_exposed_block_types_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_jira_body_above'
  $field_bases['field_jira_body_above'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_body_above',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_jira_body_below'
  $field_bases['field_jira_body_below'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_body_below',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_jira_field_options'
  $field_bases['field_jira_field_options'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_field_options',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'key' => 'Issue Key',
        'fields.issuetype.description' => 'Issue Type Description',
        'fields.issuetype.iconUrl' => 'Issue Type Icon',
        'fields.issuetype.name' => 'Issue Type Name',
        'fields.project.id' => 'Project Id',
        'fields.project.key' => 'Project Key',
        'fields.project.name' => ' Project Name',
        'fields.project.avatarUrls.48x48' => 'Project Avatar',
        'fields.project.projectCategory.id' => 'Project Category Id',
        'fields.project.projectCategory.name' => 'Project Category Name',
        'fields.fixVersions.*.name' => 'Fix Version Name',
        'fields.fixVersions.*.archived' => 'Fix Version Archived',
        'fields.fixVersions.*.released' => 'Fix Version Released',
        'fields.resolution.description' => 'Resolution Description',
        'fields.resolution.name' => 'Resolution Name',
        'fields.resolutiondate' => 'Resolution Date',
        'fields.lastViewed' => 'Last Viewed',
        'fields.watches.watchCount' => 'Watch Count',
        'fields.created' => 'Created',
        'fields.priority.iconUrl' => 'Priority Icon',
        'fields.priority.name' => 'Priority Name',
        'fields.labels.*' => 'Labels',
        'fields.timeestimate' => 'Time estimate',
        'fields.versions' => 'Versions',
        'fields.assignee.name' => 'Assignee Name',
        'fields.assignee.key' => 'Assignee Key',
        'fields.assignee.emailAddress' => 'Assignee Email Address',
        'fields.assignee.avatarUrls.48x48' => 'Assignee Avatar',
        'fields.assignee.displayName' => 'Assignee Display Name',
        'fields.updated' => 'Updated',
        'fields.status.description' => 'Status Description',
        'fields.status.iconUrl' => 'Status Icon',
        'fields.status.name' => 'Status Name',
        'fields.status.id' => 'Status ID',
        'fields.components.*.id' => 'Component Ids',
        'fields.components.*.name' => 'Component Names',
        'fields.description' => 'Issue Description',
        'fields.summary' => 'Issue Summary',
        'fields.creator.name' => 'Creator Name',
        'fields.creator.key' => 'Creator Key',
        'fields.creator.emailAddress' => 'Creator Email Address',
        'fields.creator.avatarUrls.48x48' => 'Creator Avatar',
        'fields.creator.displayName' => 'Creator Display Name',
        'fields.reporter.name' => 'Reporter Name',
        'fields.reporter.key' => 'Reporter Key',
        'fields.reporter.emailAddress' => 'Reporter Email Address',
        'fields.reporter.avatarUrls.48x48' => 'Reporter Avatar',
        'fields.reporter.displayName' => 'Reporter Display Name',
        'fields.duedate' => 'Issue Due Date',
        'fields.progress.progress' => 'Issue Progress',
        'fields.progress.total' => 'Issue Progress Total',
        'fields.votes.votes' => 'Issue Votes',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_jira_fields'
  $field_bases['field_jira_fields'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_fields',
    'indexes' => array(
      'revision_id' => array(
        0 => 'revision_id',
      ),
    ),
    'locked' => 0,
    'module' => 'field_collection',
    'settings' => array(
      'hide_blank_items' => 1,
      'path' => '',
    ),
    'translatable' => 0,
    'type' => 'field_collection',
  );

  // Exported field_base: 'field_jira_jql'
  $field_bases['field_jira_jql'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_jql',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_jira_layout'
  $field_bases['field_jira_layout'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_layout',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'table' => 'Issue Table',
        'agile' => 'Agile Board',
        'release' => 'Release',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_jira_rapidview'
  $field_bases['field_jira_rapidview'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_rapidview',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_jira_sprints'
  $field_bases['field_jira_sprints'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jira_sprints',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  return $field_bases;
}
