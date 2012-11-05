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

  /**
   * @Then /^I should see the page title "([^"]*)"$/
   */
  public function iShouldSeeThePageTitle($heading) {
    parent::iShouldSeeTheHeading($heading);
    // TODO: Check if the page title is displaid with css property or attribute.
  }

  /**
   * @Given /^I should see the application table with the following <contents>:$/
   */
  public function iShouldSeeTheApplicationTableWithTheFollowingContents(TableNode $table) {
    $page = $this->getSession()->getPage();
    $table_element = $page->find('css', 'table.spaces');
    $table_rows = $table->getRows();

    if (!$table_element) {
      throw new Exception("A table with the class $class wasn't found");
    }

    $labels = $table_element->findAll('css', 'tbody tr td.label strong');
    $statuses = $table_element->findAll('xpath', '//tbody//tr//td[@class="option"]//select//option[@selected="selected"]');

    if (!$this->compareApplicationRows($labels, $table_rows, 0) || !$this->compareApplicationRows($statuses, $table_rows, 1)) {
      throw new Exception('The given application list is not equal to the current application list.');
    }
  }

  /**
   * Comparing the rows of the applications table and the table given in the
   * scenario.
   */
  public function compareApplicationRows($elements, $table_rows, $table_row_key) {
    foreach ($elements as $i => $element) {
      if (self::getText($element->getHtml()) != $table_rows[$i][$table_row_key]) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * TODO: This method should be in a class extending BrowserKitDriver.
   *
   * Strip HTML but insert spaces between elements. Taken from the comments on:
   * http://php.net/manual/en/function.strip-tags.php
   *
   * @param $html
   *   Plain HTML.
   *
   * @return
   *   Stripped contents of the HTML.
   */
  private static function getText($html) {
     // Remove HTML tags.
    $html = preg_replace ('/<[^>]*>/', ' ', $html);

    // Remove control characters.
    $html = str_replace("\r", '', $html);
    $html = str_replace("\n", ' ', $html);
    $html = str_replace("\t", ' ', $html);

    // Remove multiple spaces.
    return trim(preg_replace('/ {2,}/', ' ', $html));
  }
}
