Feature:
  Testing the aliases of a node.

  @api
  Scenario: Verify that the pathauto alias is properly created in nodes.
    Given I am logging in as "john"
      And I visit "john/node/add/blog"
      And I fill in "Title" with "Unique Title"
     When I press "edit-submit"
     Then I verify the alias of node "Unique Title" is "john/blog/unique-title"

  @api
  Scenario: Verify that the custom alias is properly created in nodes.
    Given I am logging in as "john"
      And I visit "john/node/add/blog"
      And I fill in "Title" with "Another Unique Title"
      And I uncheck the box "Generate automatic URL alias"
      And I fill in "edit-path-alias" with "unique-custom-alias"
     When I press "edit-submit"
     Then I verify the alias of node "Another Unique Title" is "john/unique-custom-alias"

  @api
  Scenario: Verify that aliases are displayed without purl in node edit form.
    Given I am logging in as "john"
     When I edit the node "Unique Title"
     Then I verify the "URL alias" value is "blog/unique-title"

  @api
  Scenario: Verify it is possible to use a duplicate purl as a node custom path.
    Given I am logging in as "john"
      And I visit "john/node/add/blog"
      And I fill in "Title" with "John Custom Alias"
      And I uncheck the box "Generate automatic URL alias"
      And I fill in "edit-path-alias" with "john"
     When I press "edit-submit"
     Then I verify the alias of node "John Custom Alias" is "john/john"
