Feature:
  Testing the viste access.

  @api
  Scenario: Testing the Vsite access to the views.
    Given I am logged in as a user with the "administrator" role
     When I visit "blog"
      And I should not see "Me and michelle obama"
     When I visit "obama/blog"
      And I should see "Me and michelle obama"
