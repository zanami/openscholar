Feature:
  Testing the RESTWS for json output from nodes.

  @api
  Scenario: Verify for the json output for a specific node.
    Given I am logged in as "admin"
     When I ask for "john/node/1.json" i look for <json>:
          | title | Jodddhn |
          | Aby   | Jodddhn |
          | llol  | Jodddhn |
