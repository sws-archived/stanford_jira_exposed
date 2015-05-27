<?php
/**
 * stanford_jira_exposedBeanPlugin.bean.type.inc
 */

class StanfordJiraExposedBean extends BeanPlugin {

  // Storage for original content.
  public $content = "";
  // Storage for wrapped bean.
  public $wrapped = NULL;
  // Conntector
  public $connector;

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

  // ***************************************************************************
  // Theme Table
  // ***************************************************************************

  /**
   * [themeTable description]
   * @return [type] [description]
   */
  public function themeTable() {
    $bean = $this->getWrapped();
    $jql = $bean->field_jira_jql->value();
    $fields = array();
    $jqlfields = array();
    $fcs = $bean->field_jira_fields->value();
    $skip = array("expand", "id", "self", "key");

    foreach ($fcs as $i => $fc_item) {
      $value = $fc_item->field_jira_field_options[LANGUAGE_NONE][0]["value"];
      if (in_array($value, $skip)) {
        $jqlfields[] = $value;
      }
      else {
        $exp = explode(".", $value);
        $jqlfields[] = $exp[1];
      }
      $fields[$value] = $value;
    }

    // Get Issues and sort them.
    $connector = $this->getConnector();
    $issues = $connector->restAPI("searchissues")->fetch(array("jql" => $jql, "fields" => $jqlfields));

    if (is_array($issues) && empty($issues)) {
      return "<p>No items found.</p>";
    }

    // If after all of that just return the original content.
    if (!$issues) {
      drupal_set_message("Could not retreive issues from Jira", "error");
      return $this->getContent();
    }

    // Setting up the table variables.
    $vars = array();
    $vars['header'] = $this->formatTableHeadings($fields);
    $vars['rows'] = $this->formatTableFields($issues, $fields);

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


  // ***************************************************************************
  // Theme Agile
  // ***************************************************************************

  /**
   * [theme_agile description]
   * @return [type] [description]
   */
  public function themeAgile() {
    $bean = $this->getWrapped();
    $jql = $bean->field_jira_jql->value();
    $sprintId = $bean->field_jira_sprints->value();
    $rapidviewId = $bean->field_jira_rapidview->value();
    $sprintInfo = new stdClass();
    $content = array();
    $connector = $this->getConnector();

    // If we have custom jql lets get stuffs.
    if ($sprintId == "custom") {
      $issues = $connector->restAPI("searchissues")->fetch(array("jql" => $jql, "max" => 1));
      if (!$issues) {
        return "<p>No issues found</p>";
      }
      $issue = array_pop($issues);
      $projectKey = $issue->fields->project->key;
      $rapid = $connector->greenhopperApi("rapidviews")->fetch(array("query" => array("projectKey" => $projectKey)));
      if (!is_array($rapid->views)) {
        return "<p>No issues found</p>";
      }
      $view = array_pop($rapid->views);
      $rapidviewId = $view->id;
    }

    if (!$rapidviewId) {
      return "<p>Could not obtain a rapidview id</p>";
    }

    $xboard = $connector->greenhopperApi("xboard")->work("allData", array("rapidViewId" => $rapidviewId));

    // @Todo: check for valid response.
    $columns = $xboard->columnsData->columns;
    $sprints = $xboard->sprintsData->sprints;
    $issues = $xboard->issuesData->issues;


    // If we are missing the sprint id lets get it from the xboard.
    if (!is_numeric($sprintId)) {
      $sprintId = $xboard->sprintsData->sprints[0]->id;
    }

    // Get the sprint info.
    foreach ($sprints as $k => $sprint) {
      if ($sprint->id == $sprintId) {
        $sprintInfo = $sprint;
        break;
      }
    }

    $content["sprint"]["#markup"] = "<div class=\"sprint-info\">";
    $content["sprint"]["#markup"] .= "<h3 class=\"sprint-name\">" . $sprintInfo->name . "</h3> ";
    $content["sprint"]["#markup"] .= "<span class=\"sprint-start\"><b>Start date:</b> " . $sprintInfo->startDate . "</span> ";
    $content["sprint"]["#markup"] .= "<span class=\"sprint-end\"><b>End date:</b> " . $sprintInfo->endDate . "</span> ";
    $content["sprint"]["#markup"] .= "</div>";
    $content["sprint"]["#weight"] = -10;

    // If there are no issues then we can just return.
    if (!is_array($issues) || empty($issues)) {
      $content["nada"]['#markup'] = "<h2>No items found.</h2>";
      return $content;
    }

    // Get the agile category information.
    // -------------------------------------------------------------------------

    $headings = array();
    foreach ($columns as $k => $column) {
      $headings[] = $column->name;
    }

    // Agile header contents.
    $content['agileheader'] = array(
      '#theme' => "agileheader",
      '#items' => $headings,
      '#weight' => -1,
    );

    // Agile body contents. (issues)
    // -------------------------------------------------------------------------

    // Fill out the columns to match the headers.
    $groups = array_fill(0, count($headings), array());

    // Add the items to each column.
    foreach ($issues as $k => $issue) {
      foreach ($columns as $i => $column) {
        if (in_array($issue->status->id, $column->statusIds)) {
          $fields = "fixVersions,issuetype,summary,components,status,priority";
          $groups[$i][] = $connector->restApi("issue")->issue($issue->id, $fields);
          break;
        }
      }
    }

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


  // ***************************************************************************
  // Release Theme
  // ***************************************************************************

  /**
   * [themeRelease description]
   * @return [type] [description]
   */
  public function themeRelease() {
    $content = array();

    $bean = $this->getWrapped();
    $jql = $bean->field_jira_jql->value();
    $fields = array();
    $jqlfields = array();
    $fcs = $bean->field_jira_fields->value();
    $skip = array("expand", "id", "self", "key");

    foreach ($fcs as $i => $fc_item) {
      $value = $fc_item->field_jira_field_options[LANGUAGE_NONE][0]["value"];

      if (in_array($value, $skip)) {
        $jqlfields[] = $value;
      }
      else {
        $exp = explode(".", $value);
        $jqlfields[] = $exp[1];
      }

      $fields[$value] = $value;
    }

    // Always add to release.
    $jqlfields[] = "fixVersions";

    // Get Issues and sort them.
    $connector = $this->getConnector();
    $issues = $connector->restAPI("searchissues")->fetch(array("jql" => $jql, "fields" => $jqlfields));
    $groups = $this->groupIssuesByFixVersion($issues);
    $weight = 0;

    // Create tables out of each group of issues.
    foreach ($groups as $groupId => $group) {

      // No empty groups please.
      if (empty($group["issues"])) {
        continue;
      }

      // Version information headings.
      $content[$groupId . "-heading"]["#markup"] = "<h3 class=\"release-name\">" . $group['info']->name . "</h3>";

      // Start Date information.
      if (isset($group['info']->startDate)) {
        $content[$groupId . "-heading"]["#markup"] .= "<span class=\"release-start-date\"><b>" . t("Start date") . "</b>: " . $group['info']->startDate . "</span>";
      }

      // Release Date information.
      if (isset($group['info']->releaseDate)) {
        $content[$groupId . "-heading"]["#markup"] .= "<span class=\"release-end-date\"><b>" . t("End date") . "</b>: " . $group['info']->releaseDate . "</span>";
      }

      // Release information.
      if (isset($group['info']->description)) {
        $content[$groupId . "-heading"]["#markup"] .= "<p class=\"release-description\"><b>" . t("Description") . "</b>: " . $group['info']->description . "</p>";
      }

      // Set the weight.
      $content[$groupId . "-heading"]["#weight"] = $weight++;

      // Setting up the table variables.
      $vars = array();
      $vars['header'] = $this->formatTableHeadings($fields);
      $vars['rows'] = $this->formatTableFields($group["issues"], $fields);

      // Change them into a renderable array.
      $content[$groupId] = array();
      $content[$groupId]['#theme'] = "table";
      $content[$groupId]['#header'] = $vars['header'];
      $content[$groupId]['#rows'] = $vars['rows'];
      $content[$groupId]['#attributes'] = array("class" => array("release-table"));
      $content[$groupId]["#weight"] = $weight++;

    }

    // Add the bodies.
    $this->addBodies($content);
    // Add the CSS
    $content['#attached']['css'][] = drupal_get_path("module" , "stanford_jira_exposed") . "/css/stanford_jira_exposed.css";
    return $content;
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
   * [getConnector description]
   * @return [type] [description]
   */
  public function getConnector() {
    if (empty($this->connector)) {
      $this->connector = new \StanfordJiraExposed\JiraConnector();
    }
    return $this->connector;
  }

  /**
   * [setConnector description]
   * @param [type] $con [description]
   */
  public function setConnector($con) {
    $this->connector = $con;
  }


  /**
   * ***************************************************************************
   * UTILITY FUNCTIONS
   * ***************************************************************************
   */

  /**
   * [formatTableHeadings description]
   * @param  [type] $fields [description]
   * @return [type]         [description]
   */
  public function formatTableHeadings($fields) {
    $rows = array();
    $connector = $this->getConnector();
    $fieldConnector = $connector->restAPI("fields");

    // Loop through the fields and get the title.
    foreach ($fields as $i => $field) {

      $parts = explode(".", $field);
      $field_name = !empty($parts[1]) ? $parts[1] : $parts[0];

      // Special Case for key.
      if ($parts[0] == "key") {
        $rows[] = t("Issue Key");
      }
      else {
        // Use the connector like this so it can use it's built in cache.
        $fieldInfo = $fieldConnector->info($field_name);
        $rows[] = check_plain($fieldInfo->name);
      }

    }

    return $rows;
  }

  /**
   * [formatTableFields description]
   * @param  [type] $issues [description]
   * @param  [type] $fields [description]
   * @return [type]         [description]
   */
  public function formatTableFields($issues, $fields) {
    $rows = array();
    $connector = $this->getConnector();
    $fieldConnector = $connector->restAPI("fields");

    foreach($issues as $k => $issue) {
      $row = array();
      foreach ($fields as $field) {
        $value = $this->dotsToArrows($field, $issue);
        $row[] = $this->formatFieldValue($value, $field, $fieldConnector);
      }
      $rows[] = $row;
    }

    return $rows;
  }

  /**
   * [dotsToArrows description]
   * @param  [type] $field [description]
   * @param  [type] $issue [description]
   * @return [type]        [description]
   */
  public function dotsToArrows($field, $issue) {
    $parts = explode(".", $field);
    $index = array_search("*", $parts);
    $value = "";

    // If the selector is getting an array of values break it up a bit.
    if ($index !== FALSE) {
      $ends = array_slice($parts, ($index + 1));
      $parts = array_slice($parts, 0, $index);
    }

    $count = count($parts);
    switch ($count) {
      case 5:
        if (isset($issue->{$parts[0]}->{$parts[1]}->{$parts[2]}->{$parts[3]}->{$parts[4]})) {
          $value = $issue->{$parts[0]}->{$parts[1]}->{$parts[2]}->{$parts[3]}->{$parts[4]};
        }
        break;

      case 4:
        if (isset($issue->{$parts[0]}->{$parts[1]}->{$parts[2]}->{$parts[3]})) {
          $value = $issue->{$parts[0]}->{$parts[1]}->{$parts[2]}->{$parts[3]};
        }
        break;

      case 3:
        if (isset($issue->{$parts[0]}->{$parts[1]}->{$parts[2]})) {
          $value = $issue->{$parts[0]}->{$parts[1]}->{$parts[2]};
        }
        break;

      case 2:
        if (isset($issue->{$parts[0]}->{$parts[1]})) {
          $value = $issue->{$parts[0]}->{$parts[1]};
        }
        break;

      case 1:
      default:
        if (isset($issue->{$parts[0]})) {
          $value = $issue->{$parts[0]};
        }
    }

    // We have an array!
    if (is_array($value)) {
      $join = array();
      $end_count = count($ends);
      foreach ($value as $more) {
        switch ($end_count) {
          case 3:
            if (isset($more->{$ends[0]}->{$ends[1]}->{$ends[2]})) {
              $join[] = $more->{$ends[0]}->{$ends[1]}->{$ends[2]};
            }
            break;

          case 2:
            if (isset($more->{$ends[0]}->{$ends[1]})) {
              $join[] = $more->{$ends[0]}->{$ends[1]};
            }
            break;

          case 1:
            if (isset($more->{$ends[0]})) {
              $join[] = $more->{$ends[0]};
            }
            break;

          default:
            if (is_scalar($more)) {
              $join[] = check_plain($more);
            }
            else if (isset($more->name)) {
              $join[] = check_plain($more->name);
            }
            else if (isset($more->id)) {
              $join[] = check_plain($more->id);
            }
            else if (isset($more->key)) {
              $join[] = check_plain($more->key);
            }
        }
      }
      $value = implode(", ", $join);
    }

    return $value;
  }

  /**
   * [formatFieldValue description]
   * @return [type] [description]
   */
  public function formatFieldValue($value, $field, $info) {

    // Nothing special needed for the key.
    if ($field == "key") {
      return $value;
    }

    $parts = explode(".", $field);
    $fieldInfo = $info->info($parts[1]);

    switch ($fieldInfo->schema->type) {
      case "datetime":
      case "date":
        $time = strtotime($value);
        $date = "";
        if ($time) {
          $date = format_date($time, 'custom', "F d, Y");
        }
        $value = $date;
        break;

      case "number":
      case "progress":
        $value = (int) check_plain($value);
        break;

      case "account":
      case "user":
      case "project":
        if ($parts[count($parts) - 1] == "48x48") {
          $value = "<img src=\"" . $value . "\" alt=\"\" title=\"\">";
        }
        else {
          $value = check_plain($value);
        }
        break;

      case "priority":
      case "issuetype":
        if ($parts[count($parts) - 1] == "iconUrl") {
          $value = "<img src=\"" . $value . "\" alt=\"\" title=\"\">";
        }
        else {
          $value = check_plain($value);
        }
        break;

      case "string":
      case "status":
      case "resolution":
      default:
        $value = check_plain($value);
    }

    return $value;

  }

  /**
   * [groupIssuesByFixVersion description]
   * @param  array  $issues [description]
   * @return [type]         [description]
   */
  public function groupIssuesByFixVersion($issues = array()) {
    $groups = array();
    $groups['none'] = array('info' => new stdClass());
    $groups['none']['info']->name = t("None");
    $groups['none']['issues'] = array();
    $connector = $this->getConnector();

    $order = array();

    foreach ($issues as $k => $issue) {

      // If not in a fix issue put into the none pile.
      if (!isset($issue->fields->fixVersions) || !is_array($issue->fields->fixVersions)) {
        $groups['none']['issues'][] = $issue;
        continue;
      }

      $versions = $issue->fields->fixVersions;
      foreach ($versions as $i => $version) {
        $groups["v" . $version->id]['info'] = $connector->restApi("version")->version($version->id);
        $groups["v" . $version->id]['issues'][] = $issue;
        $order[$version->name] = "v" . $version->id;
      }
    }

    asort($order);
    $order = array_flip(array_values($order));
    $groups = array_merge($order, $groups);
    return $groups;
  }

  /**
   * Adds the above and below bodies to the output.
   * @param [type] $content [description]
   */
  public function addBodies(&$content) {

    $original_content = $this->getContent();
    $values = array_pop($original_content['bean']);

    if (isset($values['field_jira_body_above'])) {
      $content["field_jira_body_above"] = $values['field_jira_body_above'];
      $content["field_jira_body_above"]["#weight"] = -100;
      $content["field_jira_body_above"]["#label_display"] = "hidden";
    }

    if (isset($values['field_jira_body_below'])) {
      $content["field_jira_body_below"] = $values['field_jira_body_below'];
      $content["field_jira_body_below"]["#weight"] = 100;
      $content["field_jira_body_below"]["#label_display"] = "hidden";
    }

  }


}
