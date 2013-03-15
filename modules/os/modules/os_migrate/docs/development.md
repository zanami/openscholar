OpenScholar Migration development
=================================


Links 
-----
There are plenty of other intro to Drupal Migrate documents out there.  I do not intend to recreate them.  Here is some of the reference I found useful while working on os_migrate.
* http://btmash.com/article/2011-02-25/migrating-content-using-migrate-module
* http://drupal.org/node/1513766

Classes
-------

**OSMigration**

This is not a migration class, but a set of common functions and settings that I wanted my migration classes to inherit. 

* `restrict_vsites()` Useful during development.  Let me limit content migration to a subset of vsites.  If I was working on a particular piece of content or a certain setting, migrating 1 site instead of 1600 sites per run was incredibly helpful in speeding up development.
* `d6_db_select()`, `d6_db_query()` Wrappers for `db_select()` and `db_query()` that targeted the legacy DB from which I was importing.  
- See also `d6_db_table_exists()`
- See also `d6_variable_get()`
* `is_domain()` Checks if we're using a certain domain.  This was deployed over several servers and we had some specific code to run for each of them.
* `legacy_file_exists()` Avoid file copy errors by checking that the file exists to copy.
* `os_migrate_inline_*`, Inline file migration functions.  Probably should have been in a different class in retrospect.

**Nodes**

Nodes also have an abstract class they inherit from.  AbstractNodeOSMigration sets up the common structure for nodes to migrate.  It implements functions for each of the source, destination, map, and sourcefield properties so that child Migrations can extend them.  Similarly there's also a query method that can be extended, some of the functions mentioned above will use it. 

The basic process for adding a new content type migration is to create a class that extends AbstractNodeOSMigration.  
* Its __construct() function should call parent::__construct with the content type as its parameter.  
* AbstractNodeOSMigration::query will automatically join the content_type_$type table and select all its fields.  If you would like to make additional joins, override query.
* Back in the __construct function add field mappings.  
* Extend the prepareRow function if you need to alter your data before its saved to a field.  You can also use this as a chance to get more data if you have too many tables to join to the query.  

AbstractNodeOSMigration provides the following helper functions:
* `_has_file_field()` Checks a node for field_upload
* `_prepare_file_field()` 
