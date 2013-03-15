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
* Its \_\_construct() function should call parent::__construct with the content type as its parameter.  
* AbstractNodeOSMigration::query will automatically join the content_type_$type table and select all its fields.  If you would like to make additional joins, override query.
* Back in the \_\_construct function add field mappings.  
* Extend the prepareRow function if you need to alter your data before its saved to a field.  You can also use this as a chance to get more data if you have too many tables to join to the query.  

AbstractNodeOSMigration provides the following helper functions:
* `_has_file_field()` Checks a node for field_upload
* `_prepare_file_field()` Call this during prepareRow to handle a file field
* `_map_file_field()` See above, but during mapping

os_migrate provides classes for all the stock openscholar content types.  I've made a note of any abnormalities or exceptions that happen in each of the content type migrations below:
* Announcements
* Biblio
  - Two dependent classes for biblio tables.  These tables didn't have properties that could be attached to an entity, but were easily copied.
  - Biblio doesn't use standard fields, so migrate couldn't map to them.  Instead, this data is attached to the node before it's saved during prepare().
  - {pre,post}Rollback methods were implemented to manage biblio_duplicates.
* Bio
* Blog
* Book
  - Depends on book tables and menu links being migrated first.  Both are straightforward and included in os_migrate_node_book.inc
* Class
  - Provides class and class_material migration classes.
* CV
  - There's more file related exceptions here than I expected.  There were several locations the uploaded PDF could be stored in, so sorting out which was used required some logic.
* Event
* Feed
* Gallery
  - Lots of changes here because we changed galleries.  In d6 we used a home brewed gallery.  Each image lived in an image node.  Each gallery was full of links to nodes.  In d7 we use the contrib module media_gallery.  Its gallery nodes have attached file fields for images.  
  - Most of the heavy lifting for conversion is done in prepareRow.  Basically all it does is get the list of image owns on a gallery, saves their files, and gives the list of file ids to the new gallery.
  - _construct fills in defaults for fields that weren't present in d6.
* Link
* Modal images.  
  - This isn't a normal content type that you were expecting to see.  It was created for the single image widget.  That widget has been deprecated, but its images need to be supported.
  - Migrating a file is easy enough.  The only part that was tricky was that modal images didn't have a group.  They were part of a box.  But that box was linked to a group, so we have to use that group to make sure the image ends up belonging to the right vsite.
* Page
* Person
   - field_person_photo is the first field we've encountered that uses imagefield_crop.  There was no destination class for this, so I had to write that.  
* Presentation
  - Lots of fields here.  This was one of the earlier content types so it may not use all of the conventions that were figured out along the way.
- Software
  - Projects are straightfoward
  - Releases had some of their fields merged together.
- Stub
  - Not a real class.  Just a basic content type class that I copied and pasted to extend.
- Vsite
  - Different set of requirements than other nodes.  They all depend on vsite.  If a vsite doesn't migrate, none of its child nodes should either.
  - Includes the imagefield_crop migration mentioned in person.  (Also note that some images had no coordinates in the DB.  Provided them in a function.  This is cheating but we had no better option.)
  - Converts the featured flag to sticky.  
  - prepare() creates the group.
     
**Inline Files**

Migrating inline files was the single most complex task in OpenScholar migration.  In short, files uploaded through the inline editor were a mess.  There had been several implementations along the course of D6, each of them adding files to prod.  Those files all had to become Media entities, and their references had to become media tags.

First of all, this was not possible in a single migration class.  Migration classes operate on a 1:1 ratio.  1 node -> 1 node.  You can get away with doing a little more work on the side, during prepareRow and prepare (see Gallery nodes), but it's not what migrate was designed for and gets ugly fast.

On top of that, things that aren't really files get treated as files by Media.  An iframe embedded in content is a file with no filepath, but with some html.  So those pseudo-files will need to be created.

Inline files are migrated in 3 steps.  
1. os_migrate_specialtables.  (note: this code is overly generic.  I wrote it early on and was expecting more special cases like inline files).  This creates a table for handling the special case that is drupal node content becoming files.  It gets all instances of object, embed, and iframe tags in nodes, and makes a row for them.  Those rows will be used later.
2. Inline file migration.  This class copies files from D6 and creates Media entities from them.  All classes are in os_migrate_inline.inc.  There are four classes for the different types of inline files: files, images, embeds, and HTML.  The latter uses the rows of the special table created above.
3. During node migration, node bodies are checked for links.  We can use the tables created by migration to see which of our new media files come from which legacy nodes.  To sum up how this works:
  1. During prepare row, os_migrate_inline_update runs on each node row.
  2. If os_migrate_list_inline_files finds any files, file update functions are run.  These are os_migrate_update_body_{image,files,url,body}
  3. These update functions load up a dom object from the node's body and loop over the appropriate object.  When one of those dom objects matches one of the media entities we've already created, its html is swapped out for a newly generated media tag.
  4. See the 'inline file processing' section of os_migrate.inc


**Minor Migrations**

* Comments
  - Migration of comments is mostly straightforward.  Comments are pretty simple and we didn't have enough of them for any exceptions to arise.
  - The one surprise here is that we use DynamicMigration to handle comments.  There were was one minor difference between comments of different content types.  DynamicMigration lets us define a generic migration class and register an instance of that for each content type.  Those instances are registered when each content type is migrated.
* Flags
  - Also dymanic.  










