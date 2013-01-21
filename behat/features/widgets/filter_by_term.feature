Feature:
  Testing the filter by term widget.

  @api
  Scenario: Verify that the user sees terms in the filter by term widget.
     Given I am logged in as a user with the "administrator" role
       And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
           | Vocabularies     | authors | select list |
           | Show empty terms | check   | checkbox    |
      When I visit "john/publications"
      Then I should see "Filter by term"
       And I should see the following <links>
           | Antoine de Saint-Exupéry |
           | Douglas Noël Adams       |
           | Antoine de Saint-Exupéry |

  @api
  Scenario: Verify that the number of tagged posts appended to the term name.
     Given I am logged in as a user with the "administrator" role
       And I assign the node "John F. Kennedy" to the term "Stephen William Hawking"
       And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
           | Vocabularies         | authors | select list |
           | Show empty terms     | check   | checkbox    |
           | Show number of posts | check   | checkbox    |
       And I visit "john/publications"
       And I should see "Stephen William Hawking (1)"
      When I click "Stephen William Hawking"
      Then I should see "Stephen William Hawking"
       And I should see "John F. Kennedy"
       And I should see "This class answer the main question of the semester: who was JFK?."

  @api
  Scenario: Verify the widget can show/hide the child terms.
     Given I am logged in as a user with the "administrator" role
       And I set the term "Stephen William Hawking" under the term "Antoine de Saint-Exupéry"
       And I assign the node "John F. Kennedy" to the term "Stephen William Hawking"
       And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
           | Vocabularies         | authors | select list |
           | Show empty terms     | check   | checkbox    |
           | Show child terms     | uncheck | checkbox    |
       And I visit "john/publications"
       And I should not see "Stephen William Hawking"
       And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
           | Vocabularies         | authors | select list |
           | Show empty terms     | check   | checkbox    |
           | Show child terms     | check   | checkbox    |
      And I visit "john/publications"
      And I should see "Stephen William Hawking"
