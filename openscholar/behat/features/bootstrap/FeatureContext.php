<?php

use Drupal\DrupalExtension\Context\DrupalContext;
use Behat\Behat\Context\Step\Given;
use Behat\Gherkin\Node\TableNode;
use Guzzle\Service\Client;

require 'vendor/autoload.php';

class FeatureContext extends DrupalContext {

  // Variable to pass into the last xPath expression.
  private $xpath = '';

  // The random text generated during the scenario.
  private $randomText;

  // variable with the time out in seconds.
  private $timeout = 30;

  /**
   * Initializes context.
   *
   * Every scenario gets its own context object.
   *
   * @param array $parameters.
   *   Context parameters (set them up through behat.yml or behat.local.yml).
   */
  public function __construct(array $parameters) {
    if (isset($parameters['drupal_users'])) {
      $this->drupal_users = $parameters['drupal_users'];
    }
  }

  /**
   * Authenticates a user with password from configuration.
   *
   * @Given /^I am logged in as "([^"]*)"$/
   */
  public function iAmLoggedInAs($username) {
    try {
      $password = $this->drupal_users[$username];
    } catch (Exception $e) {
      throw new Exception("Password not found for '$username'.");
    }

    // Log in.
    // Go to the user page.
    $element = $this->getSession()->getPage();
    $this->getSession()->visit($this->locatePath('/user'));
    $element->fillField('Username', $username);
    $element->fillField('Password', $password);
    $submit = $element->findButton('Log in');
    $submit->click();
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
   * @Given /^I should see the "([^"]*)" table with the following <contents>:$/
   */
  public function iShouldSeeTheTableWithTheFollowingContents($class, TableNode $table) {
    $page = $this->getSession()->getPage();
    $table_element = $page->find('css', "table.$class");
    if (!$table_element) {
      throw new Exception("A table with the class $class wasn't found");
    }

    $table_rows = $table->getRows();
    $hash = $table->getRows();
    // Iterate over each row, just so if there's an error we can supply
    // the row number, or empty values.
    foreach ($table_rows as $i => $table_row) {
      if (empty($table_row)) {
        continue;
      }
      if ($diff = array_diff($hash[$i], $table_row)) {
        throw new Exception(sprintf('The "%d" row values are wrong.', $i + 1));
      }
    }
  }

  /**
   * @When /^I clear the cache$/
   */
  public function iClearTheCache() {
    $this->getDriver()->drush('cc all');
  }


  /**
   * @Then /^I should print page$/
   */
  public function iShouldPrintPage() {
    $this->getSession()->visit($this->locatePath('/user'));
    $element = $this->getSession()->getPage();
    $element->fillField($this->getDrupalText('username_field'), 'admin');
    $element->fillField($this->getDrupalText('password_field'), 'admin');
    $submit = $element->findButton($this->getDrupalText('log_in'));
    if (empty($submit)) {
      throw new \Exception('No submit button at ' . $this->getSession()->getCurrentUrl());
    }

    // Log in.
    $submit->click();
    $element = $this->getSession()->getPage();
    print_r($element->getContent());
  }

  /**
   * @Then /^I should get:$/
   */
  public function iShouldGet(PyStringNode $string) {
    throw new PendingException();
  }

  /**
   * @Then /^I should see the images:$/
   */
  public function iShouldSeeTheImages(TableNode $table) {
    $page = $this->getSession()->getPage();
    $table_rows = $table->getRows();
    foreach ($table_rows as $rows) {
      $image = $page->find('xpath', "//img[contains(@src, '{$rows[0]}')]");
      if (!$image) {
        throw new Exception(sprintf('The image "%s" wasn\'t found in the page.', $rows[0]));
      }
    }
  }

  // Selenium area.

  /**
   * @Given /^I sleep for "([^"]*)"$/
   */
  public function iSleepFor($seconds) {
    sleep($seconds);
  }

  /**
   * @When /^I click "([^"]*)" with the class "([^"]*)"$/
   */
  public function iClickWithTheClass($linkText, $LinkClass) {
    $this->findAnyElement($linkText, $LinkClass, '/a')->click();
  }

  /**
   * @Given /^I drag&drop "([^"]*)" to "([^"]*)"$/
   */
  public function iDragDropTo($element, $destination) {
    $selenium = $this->getSession()->getDriver();
    $selenium->evaluateScript("jQuery('#{$element}').detach().prependTo('#{$destination}');");
  }

  /**
   * @Given /^I verify the element "([^"]*)" under "([^"]*)"$/
   */
  public function iVerifyTheElementUnder($element, $container) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//*[contains(@id, '{$container}')]//div[contains(@id, '{$element}')]");

    if (!$element) {
      throw new Exception(sprintf("The element with %s wasn't found in %s", $element, $container));
    }
  }

  /**
   * @Given /^I fill "([^"]*)" with "([^"]*)"$/
   */
  public function iFillWith($input, $text) {
    $page = $this->getSession()->getPage();
    if ($text == 'random text') {
      $this->randomText = $text = $this->randomString();
    }

    $page->fillField($input, $text);
  }

  public function randomString($length = 8) {
    $letters = array('a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
    'p','q','r','s','t','u','v','q','x','y','z');

    $str = '';
    for ($i = 0; $i < $length; $i++) {
      $str .= $letters[rand(0, 25)];
    }
    return $str;
  }

  /**
   * @Given /^I hover over "([^"]*)" with the class "([^"]*)"$/
   */
  public function iHoverOverWithTheClass($element, $container) {
    $selenium = $this->getSession()->getDriver();
    $page = $this->getSession()->getPage();
    $element = $this->findAnyElement($element, $container);

    if (!$element) {
      throw new Exception(sprintf("The element with %s wasn't found in %s", $element, $container));
    }

    $selenium->mouseOver($this->xpath);
  }

  /**
   * @Then /^I should wait and see "([^"]*)"$/
   */
  public function iShouldWaitAndSee($message) {
    $page = $this->getSession()->getPage();
    if ($message == 'random text') {
      $message = $this->randomText;
    }

    $i = 0;
    while ($i <= $this->timeout) {
      if ($page->find('xpath', "//*[contains(.,'{$message}')]")) {
        return;
      }
      sleep(1);
      $i++;
    }

    throw new Exception(sprintf("The message '%s' wasn't found on the screen", $message));
  }

  /**
   * Find any element that contain the text and has the css class or id
   * selector.
   */
  private function findAnyElement($text, $container, $childElement = '*') {
    $page = $this->getSession()->getPage();
    $attributes = array(
      'id',
      'class',
    );

    // Find the element wrapped under an element with the class.
    foreach ($attributes as $attribute) {
      $this->xpath = "//*[contains(@$attribute, '{$container}')]/{$childElement}[contains(., '{$text}')]";
      $element = $page->find('xpath', $this->xpath);
      if ($element) {
        return $element;
      }
    }

    // Find the element with the class.
    foreach ($attributes as $attribute) {
      $this->xpath = "//*[contains(@$attribute, '{$container}') and contains(., '{$text}')]";
      $element = $page->find('xpath', $this->xpath);
      if ($element) {
        return $element;
      }
    }

    throw new Exception(sprintf("An element containing the text %s with the class %s wasn't found", $text, $container));
  }
}
