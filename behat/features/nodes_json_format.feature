Feature:
  Testing the RESTWS for json output from nodes.

  @api
  Scenario: Verify for the json output for a specific node.
    Given I am logged in as "admin"
     When I visit "node/1.json"
     Then I verify the json format <json>:
          | title | John |
