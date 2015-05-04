<?php
/**
 * stanford_jira_exposedBeanPlugin.bean.type.inc
 */

class StanfordJiraExposedBean extends BeanPlugin {

  // Storage for original content.
  public $content = "";
  // Storage for wrapped bean.
  public $wrapped = null;

  /**
   * Overrides the view method.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {

    // Add the bean to this.
    $this->setBean($bean);

    // Store the original content.
    $this->setContent($content);

    // Switch output theme based on type not view mode.
    if (!isset($bean->field_jira_selection_type)) {
      return $content;
    }

    // Always wrap it up for safety.
    $wrap = entity_metadata_wrapper('bean', $bean);
    $this->setWrapped($wrap);

    $type = $wrap->field_jira_selection_type->value();
    $function = "theme_" . $type;
    if (!method_exists($this, $function)) {
      return $content;
    }

    return $this->{$function}();
  }

  // THEME FUNCTIONS
  // ---------------------------------------------------------------------------

  public function theme_version() {
    $bean = $this->getWrapped();
    $project = $bean->field_jira_project_id->value();
    $key = $bean->field_jira_type_key->value();
    $jql = stanford_jira_exposed_build_jql($project, 'version', $key);
    $issues = stanford_jira_exposed_get_issues($jql);

    // If after all of that just return the original content.
    if (!$issues) {
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
}
