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
}
