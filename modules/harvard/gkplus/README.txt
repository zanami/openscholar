# GK+

Customizations for Gary King's website.

# Overview

Historically, Gary King's site was one of the first OpenScholar sites, and due
to Gary's close involvement and leadership in the OpenScholar project, Gary's
personal site became a test-bed for more experimental and fancy features.

This module provides a number of miscellaneous customizations:

* Alter gary's "people" page so no pager
* Allows nodes of type 'page' to not have titles if they are in the research-interest taxonomy
* Add checkbox on nodes toggling 'minimal' theme when viewing that node
* Add checkbox on terms toggling header hidden from anon users, and disabled to auth users
* Add checkbox on terms toggling whether or not to indent term when displayed as child
* Overrides existing taxonomy theme function defaults to display grandchild terms
* Nodeorder link on taxonomy terms using the the nodeorder module

These features forthcoming:

* Updates files when gary adds a new version to a dump directory
* Miscellaneous blocks

These components have been moved to other modules in SCHOLAR-3-0:

* Adds "Edit annotation" links to node contextual links (by Jeffrey); moved to
  contextual_annotation module.

# Setup

This module assumes specific content and configurations:
1. A vsite named 'gking'
2. Enable projects gking, nodeorder, contextual_annotation
3. Change the active theme to gking
4. Create a vocabulary named 'Research Interests' with "Sortable" selected
3. Terms, child terms, and grandchild terms in the 'Research Interests' vocab
