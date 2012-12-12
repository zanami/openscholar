Feature:
  In order to see a site biblio publications
  I need to be able to issue a query via URL
  And get XML results

  @api
  Scenario: Test a query with unknown VSite, where the answer should be "unknown".
    Given I visit "john/harvard_activity_report"
    Then I should see
    """
    <?xml version="1.0" encoding="UTF-8"?>
    <response xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespacesSchemaLocation="far_response.xsd">
      <person huid="" action_status="unknown"/>
      <errorMessage></errorMessage>
    </response>
    """

  @api
  Scenario: Test a query with invalid VSite, where the answer should be "error".
    Given I visit "john/harvard_activity_report?site_url=error-site"
    Then I should see
    """
    <?xml version="1.0" encoding="UTF-8"?>
    <response xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespacesSchemaLocation="far_response.xsd">
      <person huid="" action_status="error"/>
      <errorMessage></errorMessage>
    </response>
    """
