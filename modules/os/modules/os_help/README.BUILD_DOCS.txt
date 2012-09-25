# OS Help

Consolidates Advanced Help documentation for OpenScholar modules.

This module provides:

1. Consolidated documentation for OpenScholar modules, using Advanced Help.
2. Drush command to build documentation: `drush build_docs` (alias `drush bd`).

This module's help files can be built using it's own drush command!


1. Consolidated documentation

After running `drush build_docs`, the OS Help module's /help/ directory should
contain all the advanced help files provided by OpenScholar modules. Individual
OpenScholar modules may provide three types of files:

* Advanced Help fragments in files named like `mymodule.os_help.ini.` (required)
* HTML files that share a base name with an ini file. (optional)
* Images in an image directory (optional). Note: please namespace your images.

The conventional location is in a the help directory:

  ../mymodule/
    - mymodule.info
    - mymodule.module
    - help/
      - mymodule.os_help.ini
      - mymodule.html
      - images/
        - mymodule_foobar.png

In this example module directory structure, 2 files (mymodule.os_help.ini and
mymodule.html) are found and appended to OS Help's os_help.template.ini file.
Additionally, one image (mymodule_foobar.png) is found and added to OS Help's
/help/images/ directory.

Important: As with AH, you should link to images like:

  src="path:/images/IMAGE_NAME.EXT"

And be sure to use double quotes so that 'path' gets replaced.


2. Drush command

Usage:

`drush [@site] build_docs`

Scans OpenScholar source for *.os_help.ini files and attaches them to os_help.

Options:

'--help-readme'

Displays README.BUILD_DOCS.txt, which describes expected file structure.

# See Also

* os_mailchimp module
* os_l10n module