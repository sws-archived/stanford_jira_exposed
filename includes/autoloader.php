<?php
/**
 * Register CAPx namespace with PHP's spl_autoload_register
 */
define('STANFORDJIRAEXPOSED', realpath(dirname(__FILE__)));
function stanford_jira_exposed_autoloader($class) {
  $filename = STANFORDJIRAEXPOSED . '/' . str_replace('\\', '/', $class) . '.php';
  if (file_exists($filename)) {
    include_once($filename);
  }
}
spl_autoload_register('stanford_jira_exposed_autoloader');
