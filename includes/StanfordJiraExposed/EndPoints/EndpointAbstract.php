<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints;

abstract class EndpointAbstract implements EndpointInterface {

  // Api endpoint base path
  protected $path = "";
  // Jira Username.
  protected $username = "";
  // Jira Password.
  protected $password = "";
  // The Connector Object
  protected $connector;
  // Localized runtime cache.
  protected $cache;

  // ///////////////////////////////////////////////////////////////////////////
  // ///////////////////////////////////////////////////////////////////////////

  /**
   * [__construct description]
   * @param JiraConnector $connector [description]
   */
  public function __construct(\StanfordJiraExposed\JiraConnector $connector) {
    $this->connector = $connector;
  }


  // ///////////////////////////////////////////////////////////////////////////
  // ///////////////////////////////////////////////////////////////////////////

  /**
   * [fetch description]
   * @return [type] [description]
   */
  public function fetch($options = array()) {
    $connector = $this->getConnector();

    // @todo: Cache stuff here.
    $defaults = array(
      'user' => $this->getUsername(),
      'pass' => $this->getPassword(),
      'path' => $this->getPath()
    );

    // Create a settings obj.
    $settings = array_merge($defaults, $options);
    $hash = md5(serialize($settings));

    $cache = $this->getCache($hash);
    if ($cache) {
      return $cache;
    }

    // Fetch them.
    $ch = jira_rest_get_curl_resource($settings['user'], $settings['pass'], $settings['path']);
    try {
      $results = jira_rest_curl_execute($ch);
    }
    catch(JiraRestExpection $e) {
      return FALSE;
    }

    $this->setCache($hash, $results);

    // Save into categories and return.
    return $results;

  }

  // ///////////////////////////////////////////////////////////////////////////
  // ///////////////////////////////////////////////////////////////////////////

  /**
   * [getPath description]
   * @return [type] [description]
   */
  public function getPath() {
    return $this->path;
  }

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
   * [getConnector description]
   * @return [type] [description]
   */
  public function getConnector() {
    return $this->connector;
  }

  /**
   * [setCache description]
   * @param [type] $hash   [description]
   * @param [type] $values [description]
   */
  public function setCache($hash, $values) {
    $this->cache[$hash] = $values;
  }

  /**
   * [getCache description]
   * @param  [type] $hash [description]
   * @return [type]       [description]
   */
  public function getCache($hash) {
    if (isset($this->cache[$hash])) {
      return $this->cache[$hash];
    }
    return FALSE;
  }

}
