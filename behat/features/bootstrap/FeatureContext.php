<?php

use Drupal\DrupalExtension\Context\DrupalContext;
use Behat\Behat\Context\Step\Given;
use Behat\Gherkin\Node\TableNode;
use Guzzle\Service\Client;

require 'vendor/autoload.php';

class FeatureContext extends DrupalContext {

  // Variable to pass into the last xpath expression.
  private $xpath = '';

  // The random text generated during the scenario.
  private $randomText;

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
   * @Given /^I print page$/
   */
  public function iPrintPage() {
    $page = $this->getSession()->getPage();
    $body = $page->find('css', 'body');
    print_r($body->getHtml());
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
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//a[contains(@class, '{$LinkClass}') and .='{$linkText}']");

    if (!$element) {
      throw new Exception(sprintf("A link with the text %s and the class %s wasn't found", $LinkClass, $LinkClass));
    }

    $element->click();
  }

  /**
   * @Given /^I drag&drop "([^"]*)" to "([^"]*)"$/
   */
  public function iDragDropTo($element, $destination) {
    $selenium = $this->getSession()->getDriver();
    $selenium->evaluateScript("jQuery('#{$element}').detach().prependTo('#{$destination}');");
  }

  /**
   * @Given /^I login as "([^"]*)"$/
   */
  public function iLoginAs($userArguments) {
    $userArguments = explode('/', $userArguments);
    $page = $this->getSession()->getPage();

    $page->find('xpath', '//a[.="Admin Login"]')->click();

    $page->fillField('name', $userArguments[0]);
    $page->fillField('pass', $userArguments[1]);

    $page->findButton('edit-submit')->click();
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
   * @Given /^I Wait for the text "([^"]*)"$/
   */
  public function iWaitForTheText($message) {
    $page = $this->getSession()->getPage();
    if ($message == 'random text') {
      $message = $this->randomText;
    }

    $timeout = 30;
    $i = 0;
    while ($i <= $timeout) {
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
  private function findAnyElement($text, $container) {
    $page = $this->getSession()->getPage();
    $attributes = array(
      'id',
      'class',
    );

    // Find the element wrapped under an element with the class.
    foreach ($attributes as $attribute) {
      $this->xpath = "//*[contains(@$attribute, '{$container}')]/*[contains(., '{$text}')]";
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
