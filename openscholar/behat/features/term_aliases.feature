Feature:
  Testing the aliases of a node.

  @api
  Scenario: Verify that the pathauto alias is properly created in terms.
    Given I am logging in as "john"
      And I visit "cp/build/taxonomy/science_personal1/add"
      And I fill in "Name" with "Energy"
     When I press "edit-submit"
     Then I verify the alias of term "Energy" is "john/science/energy"

  @api
  Scenario: Verify that the custom alias is properly created in nodes.
    Given I am logging in as "john"
      And I visit "cp/build/taxonomy/science_personal1/add"
      And I fill in "Name" with "Atom"
      And I uncheck the box "Generate automatic URL alias"
      And I fill in "edit-path-alias" with "atom-custom-path"
     When I press "edit-submit"
     Then I verify the alias of term "Atom" is "john/atom-custom-path"

  @api
  Scenario: Verify that aliases are displayed without purl in node edit form.
    Given I am logging in as "john"
     When I edit the term "Energy"
     Then I verify the "URL alias" value is "science/energy"

  @api
  Scenario: Verify it is possible to use a duplicate purl as a term custom path.
    Given I am logging in as "john"
      And I visit "cp/build/taxonomy/science_personal1/add"
      And I fill in "Name" with "John Custom Alias Term"
      And I uncheck the box "Generate automatic URL alias"
      And I fill in "edit-path-alias" with "john"
     When I press "edit-submit"
     Then I verify the alias of term "John Custom Alias Term" is "john/john"
