<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints\Rest;
use StanfordJiraExposed\Endpoints\EndpointAbstract;

class VersionEndpoint extends EndpointAbstract {

  // Path;
  protected $path = "/version";


  /**
   * [issue description]
   * @param  [type] $issueId [description]
   * @return [type]          [description]
   */
  public function version($versionId) {

    if (!is_numeric($versionId)) {
      throw new \Exception("Issue ID must be numeric");
    }

    $path = $this->path . "/" . $versionId;
    return $this->fetch(array("path" => $path));
  }

}
