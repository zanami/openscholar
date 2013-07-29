Feature:
  Testing the simple view widget.

  @api
  Scenario: Verify the simple view widget works after tagging node to term.
     Given I am logging in as "john"
       And the widget "Simple view list" is set in the "Classes" page with the following <settings>:
        | Only vocabularies    | check   | checkbox    |
        | only_vocabs_values[] | authors | select list |
      When I assign the node "John F. Kennedy" to the term "Stephen William Hawking"
       And I visit "john/classes"
      Then I should see "Simple view list"
       And I should see "John F. Kennedy"

  @api
  Scenario: Verify the simple view widget works after tagging node to term.
     Given I am logging in as "john"
      When I assign the node "First blog" with the type "blog" to the term "Stephen William Hawking"
       And I assign the node "First blog" with the type "blog" to the term "Air"
      When the widget "Simple view list" is set in the "Classes" page with the following <settings>:
        | Only vocabularies     | uncheck                       | checkbox    |
        | vocabs[]              | Air, Stephen William Hawking  | select list |
       And I visit "john/classes"
      Then I should see "First blog"

