<?php
/**
 * @file
 */

namespace StanfordJiraExposed;
// Rest API.
use StanfordJiraExposed\Endpoints\Rest\StatusCategoryEndpoint;
use StanfordJiraExposed\Endpoints\Rest\StatusTypeEndpoint;
use StanfordJiraExposed\Endpoints\Rest\SearchIssuesEndpoint;
use StanfordJiraExposed\Endpoints\Rest\FieldsEndpoint;
use StanfordJiraExposed\Endpoints\Rest\IssueEndpoint;
use StanfordJiraExposed\Endpoints\Rest\VersionEndpoint;
// Greenhopper API.
use StanfordJiraExposed\Endpoints\Greenhopper\ProjectListEndpoint;
use StanfordJiraExposed\Endpoints\Greenhopper\RapidviewEndpoint;
use StanfordJiraExposed\Endpoints\Greenhopper\RapidviewsEndpoint;
use StanfordJiraExposed\Endpoints\Greenhopper\SprintEndpoint;
use StanfordJiraExposed\Endpoints\Greenhopper\XBoardEndpoint;
use StanfordJiraExposed\Endpoints\Greenhopper\VersionsEndpoint;



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
    $base = "/rest/api/latest";
    $ep = FALSE;
    $cache = $this->getCache();

    if (isset($cache['rest'][$type])) {
      return $cache['rest'][$type];
    }

    switch ($type) {

      case "statuscategory":
      case "statuscategories":
        $ep = new StatusCategoryEndpoint($this, $base);
        break;

      case "statustype":
      case "statustypes":
        $ep = new StatusTypeEndpoint($this, $base);
        break;

      case "searchissues":
        $ep = new SearchIssuesEndpoint($this, $base);
        break;

      case "fields":
      case "field":
        $ep = new FieldsEndpoint($this, $base);
        break;

      case "issue":
        $ep = new IssueEndpoint($this, $base);
        break;

      case "version":
        $ep = new VersionEndpoint($this, $base);
        break;

      default:
        throw new \Exception("Error Processing Request");
      break;

    }

    $cache['rest'][$type] = $ep;
    $this->setCache($cache);

    return $ep;
  }


  /**
   * Fetches API stuffs.
   */

  public function greenhopperAPI($type) {
    $base = "/rest/greenhopper/1.0";
    $ep = FALSE;
    $cache = $this->getCache();

    if (isset($cache['greenhopper'][$type])) {
      return $cache['greenhopper'][$type];
    }

    switch ($type) {

      case "rapidview":
        $ep = new RapidviewEndpoint($this, $base);
        break;


      case "rapidviews":
        $ep = new RapidviewsEndpoint($this, $base);
        break;

      case "sprint":
        $ep = new SprintEndpoint($this, $base);
        break;

      case "xboard":
        $ep = new XboardEndpoint($this, $base);
        break;

      case "versions":
        $ep = new VersionsEndpoint($this, $base);
        break;

      default:
        throw new \Exception("Error Processing Request");
      break;

    }

    $cache['greenhopper'][$type] = $ep;
    $this->setCache($cache);

    return $ep;
  }

  // ///////////////////////////////////////////////////////////////////////////
  // ///////////////////////////////////////////////////////////////////////////

  /**
   * [setCache description]
   * @param [type] $cache [description]
   */
  protected function setCache($cache) {
    $this->cache = $cache;
  }

  /**
   * [getCache description]
   * @return [type] [description]
   */
  protected function getCache() {
    if (!is_array($this->cache)) {
      $this->cache = array();
    }
    return $this->cache;
  }

}
