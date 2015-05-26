<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints\GreenHopper;
use StanfordJiraExposed\Endpoints\EndpointAbstract;

class RapidviewsEndpoint extends EndpointAbstract {

  // Path;
  protected $path = "/rapidviews";

  /**
   * [list description]
   * @param  array  $args [description]
   * @return [type]       [description]
   */
  public function fetch($args = array()) {
    $args['path'] = $this->path . "/list";
    return parent::fetch($args);
  }

  /**
   * [recent description]
   * @return [type] [description]
   */
  public function recent() {
    // TODO THIS>
  }

  /**
   * [viewsData description]
   * @return [type] [description]
   */
  public function viewsData() {
    // todo this
  }

  /**
   * [config description]
   * @return [type] [description]
   */
  public function config() {
    // todo this
  }

}
