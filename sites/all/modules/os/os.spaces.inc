<?php

/**
 * Implementation of hook_spaces_presets().
 */
function os_spaces_presets() {
  $export = array();

  $spaces_presets = new stdClass;
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'project';
  $spaces_presets->title = 'Project Web Site preset';
  $spaces_presets->description = 'Project site preset';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'spaces_features' => array(
        'scholar_front' => 1,
        'scholar_pages' => 1,
        'scholar_announcements' => 1,
        'scholar_events' => 1,
        'scholar_profiles' => 1,
        'scholar_publications' => 0,
        'scholar_classes' => 0,
        'scholar_image_gallery' => 0,
        'scholar_reader' => 0,
        'scholar_blog' => 0,
        'scholar_dvn' => 0,
        'scholar_links' => 0,
        'scholar_software' => 0,
      ),
      'spaces_og_selective' => 3,
      'spaces_og_register' => 0,
      'spaces_og_directory' => 1,
      'spaces_og_private' => 0,
      'site_frontpage' => 'home',
      'default_theme' => 'scholar_airstream',
      'vsite_design_maintheme' => 'scholar_airstream',
      'spaces_preset_og' => 'project',
    ),
    'context' => array(
      'openscholar-feature-biocv:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-announcements:reaction:block' => array(
        'blocks' => array(
          'boxes-scholar_announcements_latest' => array(
            'module' => 'boxes',
            'delta' => 'scholar_announcements_latest',
            'weight' => -21,
            'region' => 'right',
            'label' => 'Latest Announcements',
          ),
          'vsite_taxonomy-0' => array(
            'module' => 'vsite_taxonomy',
            'delta' => '0',
            'weight' => -20,
            'region' => 'right',
            'label' => 'Scholar site categories',
            'type' => 'context_ui',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-blog:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 37,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 38,
            'region' => 'right',
          ),
          'views-scholar_blog-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_blog-block_1',
            'weight' => 39,
            'region' => 'right',
            'status' => '0',
            'label' => 'scholar_blog: Block',
            'type' => 'context_ui',
          ),
          'views-scholar_blog_comments-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_blog_comments-block_1',
            'weight' => 40,
            'region' => 'right',
            'status' => '0',
            'label' => 'scholar_blog_comments: Recent comments',
            'type' => 'context_ui',
          ),
          'scholar_blog-blog_rss' => array(
            'module' => 'boxes',
            'delta' => 'scholar_blog-blog_rss',
            'weight' => 20,
            'region' => 'footer',
            'label' => 'RSS Feed',
          ),
        ),
      ),
      'openscholar-feature-book:reaction:block' => array(
        'blocks' => array(
          'boxes-scholar_book_current_toc' => array(
            'module' => 'boxes',
            'delta' => 'scholar_book_current_toc',
            'weight' => 20,
            'region' => 'right',
            'status' => '0',
            'label' => 'Table of Contents',
            'tags' => array(
              0 => 'Content',
              1 => 'Book',
            ),
          ),
          'boxes-scholar_book_recent_docs' => array(
            'module' => 'boxes',
            'delta' => 'scholar_book_recent_docs',
            'weight' => 21,
            'region' => 'right',
            'label' => 'Recent Documents',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 21,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-classes:reaction:block' => array(
        'blocks' => array(
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 37,
            'region' => 'right',
          ),
          'boxes-scholar_book_current_toc' => array(
            'module' => 'boxes',
            'delta' => 'scholar_book_current_toc',
            'weight' => 38,
            'region' => 'right',
            'label' => 'Table of Contents',
            'tags' => array(
              0 => 'Content',
              1 => 'Book',
            ),
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 39,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-dvn:reaction:block' => array(
        'blocks' => array(),
      ),
      'openscholar-feature-events:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 21,
            'region' => 'right',
          ),
          'scholar_events-mini' => array(
            'module' => 'scholar_events',
            'delta' => 'mini',
            'weight' => 22,
            'region' => 'right',
            'status' => '0',
            'label' => 'Mini Calendar - A small calendar used to display your sites upcoming events',
            'type' => 'context_ui',
          ),
          'views-scholar_events_upcoming-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_events_upcoming-block_1',
            'weight' => 23,
            'region' => 'right',
            'status' => '0',
            'label' => 'Upcoming events',
            'type' => 'context_ui',
          ),
        ),
      ),
      'openscholar-feature-front:reaction:block' => array(
        'blocks' => array(
          'scholar_front-get_started' => array(
            'module' => 'scholar_front',
            'delta' => 'get_started',
            'weight' => -1,
            'region' => 'admin_area',
            'status' => '0',
            'label' => 'Get Started (admin)',
          ),
          'views-scholar_events_upcoming-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_events_upcoming-block_1',
            'weight' => 20,
            'region' => 'right',
            'status' => '0',
            'label' => 'Upcoming events',
            'type' => 'context_ui',
          ),
          'boxes-scholar_announcements_latest' => array(
            'module' => 'boxes',
            'delta' => 'scholar_announcements_latest',
            'weight' => 21,
            'region' => 'right',
            'status' => '0',
            'label' => 'Latest Announcements',
          ),
          'views-vsite_featuredposts-block_1' => array(
            'module' => 'views',
            'delta' => 'vsite_featuredposts-block_1',
            'weight' => 18,
            'region' => 'right',
            'label' => 'Featured posts',
          ),
          'boxes-scholar_biocv_bio' => array(
            'module' => 'boxes',
            'delta' => 'scholar_biocv_bio',
            'weight' => 0,
            'region' => 'content_top',
            'label' => 'Bio Teaser',
          ),
        ),
      ),
      'openscholar-feature-image_gallery:reaction:block' => array(
        'blocks' => array(
          'boxes-image_gallery_recent_images' => array(
            'module' => 'boxes',
            'delta' => 'image_gallery_recent_images',
            'label' => 'Recent Images',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-links:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-pages:reaction:block' => array(
        'blocks' => array(),
      ),
      'openscholar-feature-presentations:reaction:block' => array(
        'blocks' => array(
          'views-scholar_presentations-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_presentations-block_1',
            'weight' => 38,
            'region' => 'right',
            'status' => '0',
            'label' => 'Recent Presentations',
            'type' => 'context_ui',
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 39,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 40,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-profiles:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-reader:reaction:block' => array(
        'blocks' => array(
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-scholar_reader-latest_feed_items' => array(
            'module' => 'boxes',
            'delta' => 'scholar_reader-latest_feed_items',
            'weight' => 21,
            'region' => 'right',
            'status' => '0',
            'label' => 'Latest Feed Items',
          ),
          'boxes-scholar_reader-feeds_list' => array(
            'module' => 'boxes',
            'delta' => 'scholar_reader-feeds_list',
            'weight' => 22,
            'region' => 'right',
            'status' => '0',
            'label' => 'Feeds List',
          ),
        ),
      ),
      'openscholar-feature-software:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
          'views-scholar_software_releases-block_3' => array(
            'module' => 'views',
            'delta' => 'scholar_software_releases-block_3',
            'weight' => 20,
            'region' => 'right',
            'status' => '0',
            'label' => 'Software Releases',
            'type' => 'context_ui',
          ),
          'views-scholar_software_releases-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_software_releases-block_1',
            'weight' => 20,
            'region' => FALSE,
            'status' => '0',
            'label' => 'Recommended Software Releases',
            'type' => 'context_ui',
          ),
        ),
      ),
      'openscholar-feature-none:reaction:block' => array(
        'blocks' => array(),
      ),
      'openscholar-feature-taxonomy:reaction:block' => array(
        'blocks' => array(
          'vsite_taxonomy-filter_feature' => array(
            'module' => 'vsite_taxonomy',
            'delta' => 'filter_feature',
            'weight' => 0,
            'region' => 'right',
            'status' => '0',
            'label' => 'Categories - Filter categorized content by type',
            'type' => 'context_ui',
          ),
        ),
      ),
      'openscholar-feature-publications:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-scholar_publications_recent' => array(
            'module' => 'boxes',
            'delta' => 'scholar_publications_recent',
            'weight' => 21,
            'region' => 'right',
            'label' => 'Recent publications',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 22,
            'region' => 'right',
          ),
        ),
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Project Web Site preset');
  t('Project site preset');
  $export['project'] = $spaces_presets;

  $spaces_presets = new stdClass;
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'scholar';
  $spaces_presets->title = 'OS personal';
  $spaces_presets->description = 'OpenScholar personal site';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'spaces_features' => array(
        'scholar_announcements' => 0,
        'scholar_publications' => 1,
        'scholar_front' => 1,
        'scholar_biocv' => 1,
        'scholar_classes' => 1,
        'scholar_pages' => 1,
        'scholar_image_gallery' => 0,
        'scholar_blog' => 0,
        'scholar_reader' => 0,
        'scholar_dvn' => 0,
        'scholar_links' => 0,
        'scholar_software' => 0,
      ),
      'spaces_og_selective' => 3,
      'spaces_og_register' => 0,
      'spaces_og_directory' => 1,
      'spaces_og_private' => 0,
      'site_frontpage' => 'home',
      'default_theme' => 'scholar_airstream',
      'vsite_design_maintheme' => 'scholar_airstream',
      'spaces_preset_og' => 'scholar',
    ),
    'context' => array(
      'openscholar-feature-biocv:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-announcements:reaction:block' => array(
        'blocks' => array(
          'boxes-scholar_announcements_latest' => array(
            'module' => 'boxes',
            'delta' => 'scholar_announcements_latest',
            'weight' => -21,
            'region' => 'right',
            'label' => 'Latest Announcements',
          ),
          'vsite_taxonomy-0' => array(
            'module' => 'vsite_taxonomy',
            'delta' => '0',
            'weight' => -20,
            'region' => 'right',
            'label' => 'Scholar site categories',
            'type' => 'context_ui',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-blog:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 37,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 38,
            'region' => 'right',
          ),
          'views-scholar_blog-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_blog-block_1',
            'weight' => 39,
            'region' => 'right',
            'status' => '0',
            'label' => 'scholar_blog: Block',
            'type' => 'context_ui',
          ),
          'views-scholar_blog_comments-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_blog_comments-block_1',
            'weight' => 40,
            'region' => 'right',
            'status' => '0',
            'label' => 'scholar_blog_comments: Recent comments',
            'type' => 'context_ui',
          ),
          'scholar_blog-blog_rss' => array(
            'module' => 'boxes',
            'delta' => 'scholar_blog-blog_rss',
            'weight' => 20,
            'region' => 'footer',
            'label' => 'RSS Feed',
          ),
        ),
      ),
      'openscholar-feature-book:reaction:block' => array(
        'blocks' => array(
          'boxes-scholar_book_current_toc' => array(
            'module' => 'boxes',
            'delta' => 'scholar_book_current_toc',
            'weight' => 20,
            'region' => 'right',
            'status' => '0',
            'label' => 'Table of Contents',
            'tags' => array(
              0 => 'Content',
              1 => 'Book',
            ),
          ),
          'boxes-scholar_book_recent_docs' => array(
            'module' => 'boxes',
            'delta' => 'scholar_book_recent_docs',
            'weight' => 21,
            'region' => 'right',
            'label' => 'Recent Documents',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 21,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-classes:reaction:block' => array(
        'blocks' => array(
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 37,
            'region' => 'right',
          ),
          'boxes-scholar_book_current_toc' => array(
            'module' => 'boxes',
            'delta' => 'scholar_book_current_toc',
            'weight' => 38,
            'region' => 'right',
            'label' => 'Table of Contents',
            'tags' => array(
              0 => 'Content',
              1 => 'Book',
            ),
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 39,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-dvn:reaction:block' => array(
        'blocks' => array(),
      ),
      'openscholar-feature-events:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 21,
            'region' => 'right',
          ),
          'scholar_events-mini' => array(
            'module' => 'scholar_events',
            'delta' => 'mini',
            'weight' => 22,
            'region' => 'right',
            'status' => '0',
            'label' => 'Mini Calendar - A small calendar used to display your sites upcoming events',
            'type' => 'context_ui',
          ),
          'views-scholar_events_upcoming-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_events_upcoming-block_1',
            'weight' => 23,
            'region' => 'right',
            'status' => '0',
            'label' => 'Upcoming events',
            'type' => 'context_ui',
          ),
        ),
      ),
      'openscholar-feature-front:reaction:block' => array(
        'blocks' => array(
          'scholar_front-get_started' => array(
            'module' => 'scholar_front',
            'delta' => 'get_started',
            'weight' => -1,
            'region' => 'admin_area',
            'status' => '0',
            'label' => 'Get Started (admin)',
          ),
          'views-scholar_events_upcoming-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_events_upcoming-block_1',
            'weight' => 20,
            'region' => 'right',
            'status' => '0',
            'label' => 'Upcoming events',
            'type' => 'context_ui',
          ),
          'boxes-scholar_announcements_latest' => array(
            'module' => 'boxes',
            'delta' => 'scholar_announcements_latest',
            'weight' => 21,
            'region' => 'right',
            'status' => '0',
            'label' => 'Latest Announcements',
          ),
          'views-vsite_featuredposts-block_1' => array(
            'module' => 'views',
            'delta' => 'vsite_featuredposts-block_1',
            'weight' => 18,
            'region' => 'right',
            'label' => 'Featured posts',
          ),
          'boxes-scholar_biocv_bio' => array(
            'module' => 'boxes',
            'delta' => 'scholar_biocv_bio',
            'weight' => 0,
            'region' => 'content_top',
            'label' => 'Bio Teaser',
          ),
        ),
      ),
      'openscholar-feature-image_gallery:reaction:block' => array(
        'blocks' => array(
          'boxes-image_gallery_recent_images' => array(
            'module' => 'boxes',
            'delta' => 'image_gallery_recent_images',
            'label' => 'Recent Images',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-links:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-pages:reaction:block' => array(
        'blocks' => array(),
      ),
      'openscholar-feature-presentations:reaction:block' => array(
        'blocks' => array(
          'views-scholar_presentations-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_presentations-block_1',
            'weight' => 38,
            'region' => 'right',
            'status' => '0',
            'label' => 'Recent Presentations',
            'type' => 'context_ui',
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 39,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 40,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-profiles:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
        ),
      ),
      'openscholar-feature-reader:reaction:block' => array(
        'blocks' => array(
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-scholar_reader-latest_feed_items' => array(
            'module' => 'boxes',
            'delta' => 'scholar_reader-latest_feed_items',
            'weight' => 21,
            'region' => 'right',
            'status' => '0',
            'label' => 'Latest Feed Items',
          ),
          'boxes-scholar_reader-feeds_list' => array(
            'module' => 'boxes',
            'delta' => 'scholar_reader-feeds_list',
            'weight' => 22,
            'region' => 'right',
            'status' => '0',
            'label' => 'Feeds List',
          ),
        ),
      ),
      'openscholar-feature-software:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => -20,
            'region' => 'right',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 50,
            'region' => 'right',
          ),
          'views-scholar_software_releases-block_3' => array(
            'module' => 'views',
            'delta' => 'scholar_software_releases-block_3',
            'weight' => 20,
            'region' => 'right',
            'status' => '0',
            'label' => 'Software Releases',
            'type' => 'context_ui',
          ),
          'views-scholar_software_releases-block_1' => array(
            'module' => 'views',
            'delta' => 'scholar_software_releases-block_1',
            'weight' => 20,
            'region' => FALSE,
            'status' => '0',
            'label' => 'Recommended Software Releases',
            'type' => 'context_ui',
          ),
        ),
      ),
      'openscholar-feature-none:reaction:block' => array(
        'blocks' => array(),
      ),
      'openscholar-feature-taxonomy:reaction:block' => array(
        'blocks' => array(
          'vsite_taxonomy-filter_feature' => array(
            'module' => 'vsite_taxonomy',
            'delta' => 'filter_feature',
            'weight' => 0,
            'region' => 'right',
            'status' => '0',
            'label' => 'Categories - Filter categorized content by type',
            'type' => 'context_ui',
          ),
        ),
      ),
      'openscholar-feature-publications:reaction:block' => array(
        'blocks' => array(
          'boxes-vsite_taxonomy_fltr_cat' => array(
            'module' => 'boxes',
            'delta' => 'vsite_taxonomy_fltr_cat',
            'label' => 'Filter By Category',
            'weight' => 20,
            'region' => 'right',
          ),
          'boxes-scholar_publications_recent' => array(
            'module' => 'boxes',
            'delta' => 'scholar_publications_recent',
            'weight' => 21,
            'region' => 'right',
            'label' => 'Recent publications',
          ),
          'boxes-os_addthis' => array(
            'module' => 'boxes',
            'delta' => 'os_addthis',
            'label' => 'AddThis Button',
            'weight' => 22,
            'region' => 'right',
          ),
        ),
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('OS personal');
  t('OpenScholar personal site');
  $export['scholar'] = $spaces_presets;

  return $export;
}
