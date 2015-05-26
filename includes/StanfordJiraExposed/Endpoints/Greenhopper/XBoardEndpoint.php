<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints\GreenHopper;
use StanfordJiraExposed\Endpoints\EndpointAbstract;

class XBoardEndpoint extends EndpointAbstract {

  // Path;
  protected $path = "/xboard";

  /**
   * [config description]
   * @return [type] [description]
   */
  public function config($rapidViewId, $returnDefaultBoard = TRUE) {

  }

  /**
   * [issue description]
   * @param  [type] $args [description]
   * @return [type]       [description]
   */
  public function issue($type, $args) {

    switch ($type) {
      case 'details':
        break;

      case 'attachments':
        break;

      case 'transitions':
        break;

      case "matchesBoard":
        break;

    }

  }

  /**
   * [plan description]
   * @param  [type] $args [description]
   * @return [type]       [description]
   */
  public function plan($type, $args) {
    switch ($type) {
      case "backlog/data":
        break;

      case "backlog/epics":
        break;

      case "backlog/issue":
        break;

      case "backlog/versions":
        break;

      case "sprints/actions":
        break;
    }
  }

  /**
   * [selectorData description]
   * @return [type] [description]
   */
  public function selectorData() {

  }

  /**
   * [toolSelectors description]
   * @return [type] [description]
   */
  public function toolSelectors() {

  }

  /**
   * [work description]
   * @param  [type] $args [description]
   * @return [type]       [description]
   */
  public function work($type, $args) {

    switch ($type) {
      /**
       * Args:
       * rapidViewId
       * activeQuickFilters
       * activeSprints
       * etag
       */
      case "allData":
        $path = $this->path . "/work/allData?rapidViewId=" . $args['rapidViewId'];
        $results = $this->fetch(array("path" => $path));
        break;

      case "issue":
        break;
    }

    return $results;
  }

}
