<?php
/**
 * stanford_jira_exposedBeanPlugin.bean.type.inc
 */

class StanfordJiraExposedBean extends BeanPlugin {

  // Storage for original content.
  public $content = "";
  // Storage for wrapped bean.
  public $wrapped = NULL;
  // Storage for status categories.
  public $statusCategories = array();
  // Storage for status types.
  public $statusTypes = array();

  /**
   * Overrides the view method.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {

    // Add the bean to this.
    $this->setBean($bean);

    // Store the original content.
    $this->setContent($content);

    // Switch output theme based on type not view mode.
    if (!isset($bean->field_jira_layout)) {
      return $content;
    }

    // Always wrap it up for safety.
    $wrap = entity_metadata_wrapper('bean', $bean);
    $this->setWrapped($wrap);

    // Get the theme function.
    $type = $wrap->field_jira_layout->value();
    $function = "theme" . ucfirst($type);
    if (!method_exists($this, $function)) {
      drupal_set_message("Theme function not found", "error");
      return $content;
    }

    // Call it.
    return $this->{$function}();
  }

  // THEME FUNCTIONS
  // ---------------------------------------------------------------------------

  /**
   * [themeTable description]
   * @return [type] [description]
   */
  public function themeTable() {
    $bean = $this->getWrapped();
    $jql = $bean->field_jira_jql->value();
    $issues = stanford_jira_exposed_get_issues($jql);

    // If after all of that just return the original content.
    if (!$issues) {
      drupal_set_message("Could not retreive issues from Jira", "error");
      return $this->getContent();
    }

    // Setting up the table variables.
    $vars = array();
    $vars['header'] = array(t("Type"), t("Key"), t("Title"), t("Created"));
    $vars['rows'] = array();

    // Loop through the issues and pull out what we want.
    foreach ($issues as $k => $issue) {
      $created = strtotime($issue->fields->created);
      $date = format_date($created, "custom", "M d, Y");
      $vars['rows'][] = array($issue->fields->issuetype->name, $issue->key, $issue->fields->summary, $date);
    }

    // Change them into a renderable array.
    $content = array();
    $content['datatable'] = array();
    $content['datatable']['#theme'] = "table";
    $content['datatable']['#header'] = $vars['header'];
    $content['datatable']['#rows'] = $vars['rows'];

    // Add the bodies.
    $this->addBodies($content);

    // Return for rendering.
    return $content;
  }

  /**
   * [theme_agile description]
   * @return [type] [description]
   */
  public function themeAgile() {
    $bean = $this->getWrapped();
    $jql = $bean->field_jira_jql->value();
    $issues = stanford_jira_exposed_get_issues($jql);

    // If after all of that just return the original content.
    if (!$issues) {
      drupal_set_message("Could not retreive issues from Jira", "error");
      return $this->getContent();
    }

    // Get the agile category information.
    $types = $this->getStatusTypes();
    $groups = $this->sortIssuesByStatus($issues, $types);
    $group_keys = array_keys($groups);

    // Agile header contents.
    $content['agileheader'] = array(
      '#theme' => "agileheader",
      '#items' => $group_keys,
      '#weight' => -1,
    );

    // Agile body contents.
    $content['agilebody'] = array(
      '#theme' => "agilebody",
      '#groups' => $groups,
    );

    // Add the bodies.
    $this->addBodies($content);

    // Add the CSS
    $content['#attached']['css'][] = drupal_get_path("module" , "stanford_jira_exposed") . "/css/stanford_jira_exposed.css";

    // Return for rendering.
    return $content;
  }


  /**
   * Adds the above and below bodies to the output.
   * @param [type] $content [description]
   */
  public function addBodies(&$content) {

    $original_content = $this->getContent();
    $values = array_pop($original_content['bean']);
    $content["field_jira_body_above"] = $values['field_jira_body_above'];
    $content["field_jira_body_above"]["#weight"] = -100;
    $content["field_jira_body_above"]["#label_display"] = "hidden";
    $content["field_jira_body_below"] = $values['field_jira_body_below'];
    $content["field_jira_body_below"]["#weight"] = 100;
    $content["field_jira_body_below"]["#label_display"] = "hidden";

  }


  // GETTERS AND SETTERS
  // ---------------------------------------------------------------------------

  /**
   * Why this no haz already?
   * @return object BEAN
   */
  public function getBean() {
    return $this->bean;
  }

  /**
   * [setContent description]
   * @param mixed $content [description]
   */
  public function setContent($content = "") {
    $this->content = $content;
  }

  /**
   * Gives back $this->content
   * @return [type] [description]
   */
  public function getContent() {
    return $this->content;
  }

  /**
   * [setWrapped description]
   * @param [type] $wrap [description]
   */
  public function setWrapped($wrap) {
    $this->wrapped = $wrap;
  }

  /**
   * [getWrapped description]
   * @return [type] [description]
   */
  public function getWrapped() {
    return $this->wrapped;
  }

  /**
   * [getStatusCategories description]
   * @return [type] [description]
   */
  public function getStatusCategories($reset = FALSE) {

    // Get from storage if available.
    if (!empty($this->statusCategories) && !$reset) {
      return $this->statusCategories;
    }

    // Fetch them.
    $url = "/statuscategory";
    $ch = jira_rest_get_curl_resource($this->getUsername(), $this->getPassword(), $url);
    try {
      $results = jira_rest_curl_execute($ch);
    }
    catch(JiraRestExpection $e) {
      return FALSE;
    }

    // Save into categories and return.
    $this->statusCategories = $results;
    return $results;
  }

  /**
   * [getStatusTypes description]
   * @param  boolean $reset [description]
   * @return [type]         [description]
   */
  public function getStatusTypes($reset = FALSE) {
    // Get from storage if available.
    if (!empty($this->statusTypes) && !$reset) {
      return $this->statusTypes;
    }

    // Fetch them.
    $url = "/status";
    $ch = jira_rest_get_curl_resource($this->getUsername(), $this->getPassword(), $url);
    try {
      $results = jira_rest_curl_execute($ch);
    }
    catch(JiraRestExpection $e) {
      return FALSE;
    }

    // Save into categories and return.
    $this->statusTypes = $results;
    return $results;
  }

  /**
   * [getUsername description]
   * @return [type] [description]
   */
  public function getUsername() {
    return variable_get('jira_rest_username', '');
  }

  /**
   * [getPassword description]
   * @return [type] [description]
   */
  public function getPassword() {
    return variable_get('jira_rest_password', '');
  }


  /**
   * ***************************************************************************
   * UTILITY FUNCTIONS
   * ***************************************************************************
   */

  /**
   * [filterGroups description]
   * @param  [type] $items [description]
   * @param  [type] $types [description]
   * @return [type]        [description]
   */
  public function sortIssuesByStatus($items, $types) {
    $groups = array();

    // For weighting.
    $statusCategories = $this->getStatusCategories();

    // Loop through the categories so we get the correct order.
    foreach ($statusCategories as $cat) {
      foreach ($types as $type) {
        // Skip ids that don't match so that we get the proper order.

        if ($type->statusCategory->id !== $cat->id) {
          continue;
        }

        $group = $this->filterIssuesByStatusType($items, $type);
        if (!empty($group)) {
          $groups[$type->name] = $group;
        }
      }
    }

    return $groups;
  }

  /**
   * [stanford_jira_exposed_filter_issues_by_category description]
   * @param  [type] $items    [description]
   * @param  [type] $category [description]
   * @return [type]           [description]
   */
  public function filterIssuesByStatusType($items, $type) {

    foreach ($items as $i => $item) {
      if ($item->fields->status->name !== $type->name) {
        unset($items[$i]);
      }
    }

    return $items;
  }


}
