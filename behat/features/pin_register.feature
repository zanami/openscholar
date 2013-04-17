Feature:
  In order to have create a new site as a visitor i want to be redirected to
  Harvard Pinserver to be authenticated.

  @api
  Scenario: Redirected to Harvard Pinserver when trying to create a new site.
    Given the module "iqss_pinserver_register" is disabled 
      And I am an anonymous user
     When I visit "site/register"
     Then I should get a "200" HTTP response
      And I should see the text "Create your web site"

  Scenario: Redirected to Harvard Pinserver when trying to create a new site.
    Given the module "iqss_pinserver_register" is enabled 
      And I am an anonymous user
     When I visit "site/register"
     Then I should get a "301" HTTP response
     
  Scenario: Cannot create a new site via native node add menu items.
    Given the module "iqss_pinserver_register" is enabled 
      And I am an anonymous user
     When I visit <request-url>
     Then I should get a <code> HTTP response

    Examples:
      | request-url           | code |
      | "node/add/department" | 403  |
      | "node/add/personal"   | 403  |
      | "node/add/project"    | 403  |

