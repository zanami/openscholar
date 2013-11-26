Feature:
  Testing the aliases of a node.

  @api
  Scenario: Check the alias own field own the correct alias.
    Given I am logging in as "john"
      And I visit "john/node/add/blog"
      And I fill in "Title" with "Unique Title"
      And I press "edit-submit"
      And I edit the node "Unique Title"
     Then I verify the "URL alias" value is "blog/unique-title"


  @api
  Scenario: Testing shared domain with two different vsite and the same node
            title are working properly.
    Given I am logging in as "admin"
      And I define "john" domain to "lincoln.local"
      And I define "lincoln" domain to "lincoln.local"
      And I visit "http://lincoln.local/john/about"
      And I should see "Page about john"
      And I verify the url is "lincoln.local"
     When I visit "http://lincoln.local/lincoln/about"
     Then I should see "Page about lincoln"
      And I verify the url is "lincoln.local"
