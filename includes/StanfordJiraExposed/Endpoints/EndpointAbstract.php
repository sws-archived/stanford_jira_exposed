<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints;

abstract class EndpointAbstract implements EndpointInterface {

  // Api base path.
  protected $base = "";
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
  // The curl handler
  protected $ch;

  // ///////////////////////////////////////////////////////////////////////////
  // ///////////////////////////////////////////////////////////////////////////

  /**
   * [__construct description]
   * @param JiraConnector $connector [description]
   */
  public function __construct(\StanfordJiraExposed\JiraConnector $connector, $base) {
    $this->base = $base;
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

    $defaults = array(
      'user' => $this->getUsername(),
      'pass' => $this->getPassword(),
      'path' => $this->getPath(),
      'method' => "GET",
    );

    // Create a settings obj.
    $settings = array_merge($defaults, $options);

    // Append any query parts.
    if (isset($settings['query']) && is_array($settings['query'])) {
      $settings['path'] .= "?";
      foreach ($settings['query'] as $key => $value) {
        $settings['path'] .= $key . "=" . urlencode($value) . "&";
      }
    }

    // Perform prefetch operations.
    $hash = $this->getHash($settings);
    $cache = $this->preFetch($hash, $settings);
    if ($cache) {
      return $cache;
    }

    // Fetch them.
    $ch = $this->getCurlResource($settings['user'], $settings['pass'], $settings['path'], $settings['method']);
    try {
      $results = $this->jiraRestCurlExecute($ch);
    }
    catch(JiraRestExpection $e) {
      return FALSE;
    }

    // Perform post fetch activities on the results.
    $this->postFetch($hash, $results);

    // Save into categories and return.
    return $results;

  }

  // ///////////////////////////////////////////////////////////////////////////
  // ///////////////////////////////////////////////////////////////////////////

  /**
   * Handles the
   * @param  array  $settings [description]
   * @return [type]           [description]
   */
  public function preFetch($hash, &$settings) {
    // Cache hash so we don't make duplicate calls.
    $cache = $this->getCache($hash);
    if ($cache) {
      return $cache;
    }
  }

  /**
   * [postFetch description]
   * @param  [type] $hash    [description]
   * @param  [type] $results [description]
   * @return [type]          [description]
   */
  public function postFetch($hash, $results) {
    // Store the results for the query in cache in case we try to call it again.
    $this->setCache($hash, $results);
  }

  // ///////////////////////////////////////////////////////////////////////////
  // ///////////////////////////////////////////////////////////////////////////

  /**
   * [getHash description]
   * @param  [type] $settings [description]
   * @return [type]           [description]
   */
  protected function getHash($settings = array()) {
    return md5(serialize($settings));
  }

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

  /**
   * [get_base description]
   * @return [type] [description]
   */
  public function get_base() {
    return $this->base;
  }

  /**
   * [set_base description]
   * @param [type] $base [description]
   */
  public function set_base($base) {
    $this->base = $base;
  }

  /**
   * [setCurlHandler description]
   * @param [type] $ch [description]
   */
  protected function setCurlHandler($ch) {
    $this->ch = $ch;
  }

  /**
   * [getCurlHandler description]
   * @return [type] [description]
   */
  protected function getCurlHandler() {
    if (!is_null($this->ch)) {
      $this->ch = $this->getCurlResource($this->getUsername(), $this->getPassword(), $this->getPath());
    }
    return $this->ch;
  }


  /**
   * [getCurlResource description]
   * @return [type] [description]
   */
  public function getCurlResource($username, $password, $url, $method = "GET") {

    $jira_url = variable_get('jira_rest_jirainstanceurl', 'https://localhost:8443') . $this->get_base();

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $jira_url . $url);
    curl_setopt($ch, CURLOPT_USERPWD, $username . ':' . $password);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $method);

    return $ch;
  }

  /**
   * [jiraRestCurlExecute description]
   * @param  [type] $ch [description]
   * @return [type]     [description]
   */
  public function jiraRestCurlExecute($ch) {
    return jira_rest_curl_execute($ch);
  }

}
