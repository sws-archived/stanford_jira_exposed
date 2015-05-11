<?php
/**
 * Theme functions
 */

/**
 * [theme_agileheader description]
 * @param  [type] $items [description]
 * @return [type]        [description]
 */
function theme_agileheader($vars) {
  $items = $vars['items'];
  $content = "<div class=\"jira-container jira-agile-header\">";
  $base = 12;
  $count = count($vars['items']);
  $span = "span" . floor($base / $count);

  $list = array('items' => array(), 'title' => '', 'type' => 'ul', 'attributes' => array('class' => array('agile-header-list')));

  foreach ($items as $item) {
    $name = is_string($item) ? $item : $item->name;
    $list['items'][] = array("data" => "<h3>" . $name . "</h3>", 'class' => array($span));
  }

  $content .= theme_item_list($list);
  $content .= "</div>";

  return $content;
}

/**
 * [theme_agilebody description]
 * @param  [type] $items      [description]
 * @param  [type] $categories [description]
 * @return [type]             [description]
 */
function theme_agilebody($vars) {
  $content = "<div class=\"jira-container jira-agile-body\">";
  $groups = $vars['groups'];
  $base = 12;
  $count = count($groups);
  $span = "span" . floor($base / $count);

  foreach ($groups as $key => $values) {
    $list = array('items' => array(), 'title' => '', 'type' => 'ul', 'attributes' => array('class' => array('agile-body-list', $span)));
    if (count($values)) {
      foreach ($values as $k => $v) {
        $list['items'][] = array("data" => theme("agileitem", array("item" => $v)), "class" => array("agileitem"));
      }
    }
    else {
      $list['items'][] = array("data" => "", "class" => array("agileitem-empty"));
    }
    $content .= theme_item_list($list);
  }

  $content .= "</div>";
  return $content;
}


/**
 * [theme_agileitem description]
 * @param  [type] $vars [description]
 * @return [type]       [description]
 */
function theme_agileitem($vars) {
  $item = $vars['item'];

  $issue_class = drupal_clean_css_identifier($item->fields->issuetype->name);
  $issue_icon = "<img src=\"" . $item->fields->issuetype->iconUrl . "\" alt=\"" . $item->fields->issuetype->name . "\" title=\"" . $item->fields->issuetype->name . "\" />";

  $content = "<div class=\"agileitem-wrapper $issue_class\">";

    // Title.
    $content .= "<div class=\"agileitem-title\">";
      $content .= "<span class=\"agileitem-icon\">" . $issue_icon . "</span>";
      $content .= "<h4 class=\"agileitem-key\">" . $item->key . "</h4>";
    $content .= "</div>";

    // Body.
    $content .= "<div class=\"agileitem-body\">";
      $content .= "<span class=\"agileitem-summary\">" . $item->fields->summary . "</span>";
    $content .= "</div>";

    // Footer.
    $content .= "<div class=\"agileitem-footer\">";
      $content .= theme("agilecomponents", array("items" => $item->fields->components));
    $content .= "</div>";

  $content .= "</div>";
  return $content;
}

/**
 * [theme_agileitemlist description]
 * @param  [type] $vars [description]
 * @return [type]       [description]
 */
function theme_agileitemlist($vars) {

}

/**
 * [theme_agilecomponents description]
 * @param  [type] $vars [description]
 * @return [type]       [description]
 */
function theme_agilecomponents($vars) {
  $items = $vars['items'];
  $list = array('items' => array(), 'title' => '', 'type' => 'ul', 'attributes' => array('class' => array('agile-component-list')));

  foreach($items as $k => $item) {
    $list["items"][] = $item->name;
  }

  return theme_item_list($list);
}


/**
 * *****************************************************************************
 * HELPERS
 * *****************************************************************************
 */

