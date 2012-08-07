DESCRIPTION
--------------------------
Provides the search functionality for OpenScholar sites.  By default this has been configured to use a solr search service.


To setup solr go to (admin/config/search/apachesolr) and enter your solr server information.

To fall back on DB node search go to (admin/config/search/settings) and enable only the node search and switch the default to "Node" away from Solr.