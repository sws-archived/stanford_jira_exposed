<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints\Rest;
use StanfordJiraExposed\Endpoints\EndpointAbstract;

class IssueEndpoint extends EndpointAbstract {

  // Path;
  protected $path = "/issue";


  /**
   * [issue description]
   * @param  [type] $issueId [description]
   * @return [type]          [description]
   */
  public function issue($issueId, $fields = NULL) {
    if (!is_numeric($issueId)) {
      throw new \Exception("Issue ID must be numeric");
    }
    $path = $this->path . "/" . $issueId;

    if (!empty($fields)) {
      $path .= "?fields=" . $fields;
    }

    return $this->fetch(array("path" => $path));
  }

}
