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

  /**
   * [themeTable description]
   * @return [type] [description]
   */
  public function themeTable() {
    $bean = $this->getWrapped();
    $jql = $bean->field_jira_jql->value();
    // $fields = unserialize($bean->field_jira_field_storage->value());
    $fields = array();
    $fcs = $bean->field_jira_fields->value();
    foreach ($fcs as $i => $fc_item) {
      $fields[$fc_item->field_jira_field_options[LANGUAGE_NONE][0]["value"]] = $fc_item->field_jira_field_options[LANGUAGE_NONE][0]["value"];
    }

    $connector = $this->getConnector();
    $issues = $connector->restAPI("searchissues")->fetch(array("jql" => $jql));

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
          default:
            if (isset($more->{$ends[0]})) {
              $join[] = $more->{$ends[0]};
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
      case "priority":
      default:
        $value = check_plain($value);
    }

    return $value;

  }

  /**
   * [theme_agile description]
   * @return [type] [description]
   */
  public function themeAgile() {
    $bean = $this->getWrapped();
    $jql = $bean->field_jira_jql->value();
    $connector = new \StanfordJiraExposed\JiraConnector();
    $issues = $connector->restAPI("searchissues")->fetch(array("jql" => $jql));

    // If after all of that just return the original content.
    if (!$issues) {
      drupal_set_message("Could not retreive issues from Jira", "error");
      return $this->getContent();
    }

    // Get the agile category information.
    $types = $connector->restAPI("statustype")->fetch();
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
   * [filterGroups description]
   * @param  [type] $items [description]
   * @param  [type] $types [description]
   * @return [type]        [description]
   */
  public function sortIssuesByStatus($items, $types) {
    // JiraRestConnector.
    $connector = $this->getConnector();
    // For weighting.
    $statusCategories = $connector->restAPI("statuscategories")->fetch();

    // @Todo: Create way to pick the options or set a minimum instead of this.
    $groups = array(
      'Open' => array('catid' => 1), 'In Progress' => array('catid' => 2), 'Testing' => array('catid' => 4), 'Resolved' => array('catid' => 3),
    );

    // Loop through the categories so we get the correct order.
    foreach ($statusCategories as $cat) {
      foreach ($types as $type) {
        // Skip ids that don't match so that we get the proper order.

        if ($type->statusCategory->id !== $cat->id) {
          continue;
        }

        $group = $this->filterIssuesByStatusType($items, $type);

        if (!empty($group) || in_array($type->name, $groups)) {
          $groups[$type->name] = $group;
          $groups[$type->name]['catid'] = $cat->id;
        }
      }
    }

    // Custom ordering based on catId.
    $order = $this->getCategorySortOrder();
    stanford_jira_exposed_sort_by_category_key($groups, $order);
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

  /**
   * [getCategorySortOrder description]
   * @return [type] [description]
   */
  public function getCategorySortOrder() {
    $order = array();
    // JiraRestConnector.
    $connector = $this->getConnector();
    // For weighting.
    $categories = $connector->restAPI("statuscategories")->fetch();
    foreach ($categories as $i => $cat) {
      $order[] = $cat->id;
    }
    return $order;
  }

  // /rest/greenhopper/1.0/rapidview/{rapid_view_id} //// to get the information about Scrum boards
  // /rest/greenhopper/1.0/rapidviews/list
  // /rest/greenhopper/1.0/sprintquery/{rapid_view_id}?includeFutureSprints=true  //// to get the information about sprints that are included in selected Scrum board
  // /rest/greenhopper/1.0/sprints/
  // /rest/greenhopper/1.0/sprint/{sprint_id?}
  // /rest/greenhopper/1.0/rapid/charts/sprintreport?rapidViewId={rapidViewId}&sprintId={sprintID} to get list of issues for that sprint.

}
