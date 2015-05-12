<?php
/**
 * @file
 */

namespace StanfordJiraExposed;
use StanfordJiraExposed\Endpoints\StatusCategoryEndpoint;
use StanfordJiraExposed\Endpoints\StatusTypeEndpoint;
use StanfordJiraExposed\Endpoints\SearchIssuesEndpoint;
use StanfordJiraExposed\Endpoints\FieldsEndpoint;

class JiraConnector {

  // Cache runtime storage
  protected $cache = array();
  // Storage for status categories.
  protected $statusCategories = array();
  // Storage for status types.
  protected $statusTypes = array();
  // Jira Username.
  protected $username = "";
  // Jira Password.
  protected $password = "";


  // Constructor
  // ---------------------------------------------------------------------------

  public function __construct($username = NULL, $password = NULL) {

    // Set username.
    if (!is_null($username)) {
      $this->setUsername($username);
    }
    // Set password.
    if (!is_null($password)) {
      $this->setPassword($password);
    }

  }

  // Gets / Sets
  // ---------------------------------------------------------------------------

  /**
   * [getPassword description]
   * @return [type] [description]
   */
  public function getPassword() {
    $pass = $this->password;
    if (empty($pass)) {
      $pass = variable_get("jira_rest_password");
    }
    return $pass;
  }

  /**
   * [setPassword description]
   * @param [type] $pass [description]
   */
  public function setPassword($pass) {
    $this->password = $pass;
  }

  /**
   * [getUsername description]
   * @return [type] [description]
   */
  public function getUsername() {
    $name = $this->username;
    if (empty($name)) {
      $name = variable_get("jira_rest_username");
    }
    return $name;
  }

  /**
   * [setUsername description]
   * @param [type] $user [description]
   */
  public function setUsername($user) {
    $this->username = $user;
  }




  /**
   * Fetches API stuffs.
   */

  public function restAPI($type) {
    switch ($type) {

      case "statuscategory":
      case "statuscategories":
        $ep = new StatusCategoryEndpoint($this);
        break;

      case "statustype":
      case "statustypes":
        $ep = new StatusTypeEndpoint($this);
        break;

      case "searchissues":
        $ep = new SearchIssuesEndpoint($this);
        break;

      case "fields":
      case "field":
        $ep = new FieldsEndpoint($this);
        break;

      default:
        throw new Exception("Error Processing Request");
      break;

    }

    return $ep;
  }

  /**
   * [restGreenhopper description]
   * @return [type] [description]
   */
  public function restGreenhopper() {
    // Nothing here yet.
  }


}
