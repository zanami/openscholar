<?php

use Drupal\DrupalExtension\Context\DrupalContext;
use Behat\Behat\Context\Step\Given;
use Behat\Gherkin\Node\TableNode;
use Guzzle\Service\Client;

require 'vendor/autoload.php';

class FeatureContext extends DrupalContext {

  /**
   * @Given /^I print page$/
   */
  public function iPrintPage() {
    $page = $this->getSession()->getPage();
    $body = $page->find('css', 'body');
    print_r($body->getHtml());
  }

  /**
   * @Given /^I am on a "([^"]*)" page titled "([^"]*)"(?:, in the tab "([^"]*)"|)$/
   */
  public function iAmOnAPageTitled($page_type, $title, $subpage = NULL) {
    if (!in_array($page_type, array('bio'))) {
      throw new \Exception("Unknown page type '$page_type'.");
    }

    $table = 'node';
    $id = 'nid';
    $path = "$page_type/%";
    $type = str_replace('-', '_', $page_type);

    $path .= "/$subpage";

    //TODO: The title and type should be properly escaped.
    $query = "\"
      SELECT $id AS identifier
      FROM $table
      WHERE title = '$title'
      AND type = '$type'
    \"";

    $result = $this->getDriver()->drush('sql-query', array($query));
    $id = trim(substr($result, strlen('identifier')));

    if (!$id) {
      throw new \Exception("No $page_type with title '$title' was found.");
    }
    $path = str_replace('%', $id, $path);

    return new Given("I am at \"node/$id\"");
  }
}
