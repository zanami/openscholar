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

** Nodes**


