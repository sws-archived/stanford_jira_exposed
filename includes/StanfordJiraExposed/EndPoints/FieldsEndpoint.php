<?php
/**
 * @file
 */

namespace StanfordJiraExposed\Endpoints;

class FieldsEndpoint extends EndpointAbstract {

  // Path;
  protected $path = "/field";
  // Info cache;
  protected $info = array();

  /**
   * [info description]
   * @param  [type] $id [description]
   * @return [type]     [description]
   */
  public function info($id) {

    if (isset($this->info[$id])) {
      return $this->info[$id];
    }

    $fields = $this->fetch();
    foreach ($fields as $field) {
      if ($field->id == $id) {
        $this->info[$id] = $field;
        return $field;
      }
    }
  }

}
