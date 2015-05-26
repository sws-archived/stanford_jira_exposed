<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints\GreenHopper;
use StanfordJiraExposed\Endpoints\EndpointAbstract;

class SprintEndpoint extends EndpointAbstract {

  // Path;
  protected $path = "/sprint";

  /**
   * [picker description]
   * @return [type] [description]
   */
  public function picker() {
    return $this->fetch(array('path' => $this->path . "/picker"));
  }

  /**
   * [fetch description]
   * @param  [type] $args [description]
   * @return [type]       [description]
   */
  public function info($sprintId) {
    $args = array();

    // Handle sprint id.
    if (is_numeric($sprintId)) {
      $args = array("path" => $this->path . "/" . $sprintId . "/complete");
    }
    else {
      throw new \Exception("SprintId must be numeric");
    }

    return $this->fetch($args);
  }

  /**
   * [rapid description]
   * @param  [type] $rapidviewID [description]
   * @return [type]              [description]
   */
  public function rapid($rapidviewID) {
     // Handle sprint id.
    if (!is_numeric($rapidviewID)) {
      throw new \Exception("Rapidview Id must be numeric");
    }
    return $this->info($rapidviewID);
  }


}
