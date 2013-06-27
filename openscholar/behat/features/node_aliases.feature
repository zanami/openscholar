Feature:
  Testing the aliases of a node.

  @api
  Scenario: Check that the OG audience field will be shown when editing content.
    Given I am logged in as "john"
     When I editing the node "First blog"
     Then I verify the "URL alias" value is "blog/first-blog"
