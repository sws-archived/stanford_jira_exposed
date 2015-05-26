<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints\GreenHopper;
use StanfordJiraExposed\Endpoints\EndpointAbstract;

class VersionsEndpoint extends EndpointAbstract {

  // Path;
  protected $path = "/versions";

  /**
   * [info description]
   * @param  [type] $versionId [description]
   * @return [type]            [description]
   */
  public function info ($versionId) {
    if (!is_numeric($versionId)) {
      throw new \Exception("Error Processing Request", 1);
    }
    return $this->fetch(array("path" => $this->path . "/" . $versionId, "method" => "POST"));
  }

}
