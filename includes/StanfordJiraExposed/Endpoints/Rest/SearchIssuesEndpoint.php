<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints\Rest;
use StanfordJiraExposed\Endpoints\EndpointAbstract;

class SearchIssuesEndpoint extends EndpointAbstract {

  protected $path = "/search";

  /**
   * [fetch description]
   * @return [type] [description]
   */
  public function fetch($options = array()) {
    $jql = $options['jql'];
    $max = isset($options['max']) ? $options['max'] : 999;

    $options['query'] = array(
      'jql' => $jql,
      'maxResults' => $max,
    );

    // Add fields if available.
    if (!empty($options['fields'])) {
      $options['query']['fields'] = implode(",", $options['fields']);
    }

    $results = parent::fetch($options);

    if (!empty($results->issues)) {
      return $results->issues;
    }

    return array();
  }

}



