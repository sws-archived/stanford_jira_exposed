<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints;

class SearchIssuesEndpoint extends EndpointAbstract {

  /**
   * [fetch description]
   * @return [type] [description]
   */
  public function fetch($options = array()) {
    $jql = $options['jql'];
    $jira = jira_rest_searchissue($this->getUsername(), $this->getPassword(), $jql);

    if (isset($jira->issues)) {
      return $jira->issues;
    }

    throw new Exception("Could not get issues");
  }

}



