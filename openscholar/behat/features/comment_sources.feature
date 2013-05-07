Feature:
  In order to customize comment settings
  as a site admin
  i want to choose between several options.

  @api @wip
  Scenario Outline: Comments are off by default for every content type.
    Given I create a node of type <type>
     Then I should not see any comments enabled

  Examples:
    | type               |
    | "news"             |
    | "blog"             |
    | "software_project" |
    | "software_release" |

  @api @wip
  Scenario Outline: After setting comment source, the comment forms display.
    Given I set the comment source for "blog" to <source>
     When I create a node of type "blog"
     Then I should see <source> comments enabled
      And I should not see <source1> comments enabled
      And I should not see <source2> comments enabled

  Examples:
    | source      | source1     | source2     |
    | "comment"   | "disqus"    | "fb_social" |
    | "disqus"    | "fb_social" | "comment"   |
    | "fb_social" | "comment"   | "disqus"    |

  @api @wip
  Scenario Outline: Comment links on teasers display correct comment info.

  Examples:

  @api @wip
  Scenario Outline: Comments can be turned off for individual posts.

  Examples:

  @api @wip
  Scenario Outline: Posts with comments remember their comment source.

  Examples:

  @api @wip
  Scenario Outline: Posts without comments automatically use new comment source.

  Examples: