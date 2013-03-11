Feature:
  Testing the faq app.

  @api
  Scenario: Testing the migration of FAQ
    Given I am logged in as a user with the "administrator" role
      And I visit "john/faq"
      And I should see "What does JFK stands for?"
     When I click "What does JFK stands for?"
     Then I should see "JFK stands for: John Fitzgerald Kennedy."

  @api
  Scenario: Testing the migration of FAQ
    Given I am logged in as a user with the "administrator" role
      And I visit "john/faq"
      And I click "Add FAQ"
      And I fill "edit-title" with random text
      And I press "Save"
     Then I should see the random string

  @api
  Scenario: Verfity that "classes" tab shows all nodes.
    Given I visit "john/faq"
     Then I should see "What does JFK stands for?"
      And I should see "Where does JFK born?"

  @api
  Scenario: Verfity that "classes" tab shows can filter nodes by term.
     Given I visit "john/faq/science/air"
      Then I should see "What does JFK stands for?"
       And I should not see "Where does JFK born?" under "content"
