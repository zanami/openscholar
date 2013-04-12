<?php

/**

brainstorming.

separate out commands.  rebuild rebuilds from files.
add appends to files, calls rebuild, ac domain add
check
list
remove, deletes from files, rebuild, ac domain remove.

 */

/**
 * @file os.drush.inc
 *
 * Drush commands provided by OpenScholar
 */

function os_drush_command() {
  return array(      
    'os-domain-check' => array(
      'description' => dt('Checks that files exist and permissions are correct for running drush os-domain-* scripts.'),
      'options' => array(
        'path-to-htaccess' => dt('Not implemented'),
        ''
      ),
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-dc' => dt('Enable production site to use example.com domain.  The owner of example.com will still have to add that domain to her or her vsite.'),
      ),
      'aliases' => array('os-dc'),
    ),
      

    'os-domain-build' => array(
      'description' => dt('Build\'s os-domains sites.php, .htaccess, and (optionally) acquia cloud based on current lists of domains.'),
      'options' => array(
        'path-to-htaccess' => dt('Not implemented'),
        ''
      ),
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-db' => dt('Builds new copise of sites.php and .htaccess.  If acquia cloud API is available, also update their registry of domains.'),
      ),
      'aliases' => array('os-db'),
    ),
      

    'os-domain-add' => array(
      'description' => dt('Add a domain to a site.'),
      'options' => array(
        'path-to-htaccess' => dt('Not implemented'),
        'www' => dt('Add domain and www.domain'),
      ),
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-da -www example.com' => dt('Sets up @prod site to serve example.com and www.example.com.'),
      ),
      'aliases' => array('os-da'),
    ),
      
    'os-domain-remove' => array(
      'description' => dt('Remove a domain from a site.'),
      'options' => array(
        'path-to-htaccess' => dt('Not implemented'),
        'www' => dt('Remove both domain and www.domain'),
      ),
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-dr -www example.com' => dt('Sets up @prod site to stop serving example.com and www.example.com.'),
      ),
      'aliases' => array('os-dr'),
    ),
    'os-domain-help' => array(),
    'os-domain-list' => array(),
  );
}

/**
 * @function _drush_os_domain_get_server_conf() 
 * 
 * Gets common vars used in os-domain drush commands
 */
function _drush_os_domain_get_server_conf() {
  $status = drush_sitealias_get_record('@self');
  $root = $status['root'];
  $context = drush_get_context();
  
  return array(
    'root' => $root,
    'base_path' => $root . '/' . $context[DRUSH_DRUPAL_SITE_ROOT],
    'files_path' => $root . '/files',
    'domains_list' => $root . '/' . $context[DRUSH_DRUPAL_SITE_ROOT] . '/' . 'domains.list',
    'sitesphp' => $root . '/sites/sites.php',
    'acquia-cloud-api' => (isset($context[DRUSH_COMMANDS]['ac-domain-add'])),
  );
}

/**
 * @function drush_os_domain_build
 * 
 * Service hook for os-domain-build
 * 
 * Parses domains.list files into .htaccess, sites.php.inc, and (optionally) acquia cloud domain config
 */
function drush_os_domain_build() {
  $srv_conf = _drush_os_domain_get_server_conf();
  drush_invoke('os-domain-check');
  
  //build list of multisites and their domains
  $sites = $srv_conf['root'] . '/sites/';
  $multisites = array();
  foreach (scandir($sites) as $site) {
    if (is_dir($sites.$site) && file_exists($sites.$site.'/domains.list')) {
      $domains = file($sites.$site.'/domains.list');
      $domains = array_unique($domains);
      $domains = array_map('trim', $domains);
      $domains = preg_grep('/^[^#].*$/', $domains); //strip out comments
      $multisites[$site] = $domains;
    }
  }
  
  //flatten the multisites array for sites.php
  $a_sites = array();
  foreach ($multisites as $site => $domains) {
    $a_sites += array_fill_keys($domains, $site);
  }
  $inc_file= $srv_conf['root'] . '/sites/sites.php.inc';
  $export = "<?php\n\n\$sites = " . var_export($a_sites, TRUE) . ";\n";
  _os_drush_confirm_file_overwrite($export, $inc_file);
  
  // htaccess
  $htaccess = _os_drush_domain_htaccess($multisites);
  _os_drush_confirm_file_overwrite($htaccess, $srv_conf['files_path'] . '/.htaccess');
  
  
  // try acquia cloud api
//   if ($srv_conf['acquia-cloud-api']) {
//     // drush @hwpi1.dev ac-domain-list --format export
  
//     drush_invoke('ac-domain-add', array($domain));
//     drush_invoke('ac-domain-purge', array($domain));
//   }
}

/**
 * @function drush_os_domain_add
 *
 * Service hook for os-domain-add
 *
 * Adds domain(s) to domain listings, then calls rebuild.
 */
function drush_os_domain_add() {
  $args = func_get_args();
  $domain = (count($args)) ? str_replace('http://', '', $args[0]) : drush_die('drush os-domain-config requires a domain.  See drush odc --help'); 
  $srv_conf = _drush_os_domain_get_server_conf();
  drush_invoke('os-domain-check');
  
  //make sure domain is valid and correct
  if (drush_prompt("Are you sure you want to add '$domain'? (y/n)", 'n') != 'y') {
    drush_die('User bailed out');
  }
  
  //append domain to list
  drush_file_append_data($srv_conf['domains_list'], "\n".$domain);
  
  drush_invoke('os-domain-build');
}

/**
 * @function drush_os_domain_check
 *
 * Command callback for os-domain-check
 * 
 * Checks configuration and bails out if drush os-domain commands aren't going to work
 */
function drush_os_domain_check() {
  $srv_conf = _drush_os_domain_get_server_conf();
  
  //check for domains.list
  if (!file_exists($srv_conf['domains_list'])) {
    file_put_contents($srv_conf['domains_list'], "# This file created automatically by drush os-domain-config.  Do not delete.\n") || drush_die('Error creating domains.list.  Check your permissions.');
    file_put_contents($srv_conf['domains_list'], $srv_conf['uri'], FILE_APPEND);
  }
  
  //do we have sites.php?
  if (!file_exists($srv_conf['sitesphp'])) {
    drush_die('sites.php is missing.  Please create a sites.php file to use os-domain-config.');
  }
  
  //check if sites.php has our include line
  $contents = file_get_contents($srv_conf['sitesphp']);
  if (!preg_match('/require_once.*sites\.php\.inc/', $contents)) {
    $required = <<<EOF
    
// Read in include files from our domains.
\$dir = dirname(__FILE__);
if (file_exists(\$dir . '/sites.php.inc')) {
  require_once(\$dir . '/sites.php.inc');
}
    
EOF;
    drush_die('sites.php exists, but is not configured to include our domain overrides.  Please add the following code before $sites is initalized.' . $required);
  }
  
  //can we use our files dirs' .htaccess?
  if (!is_dir($srv_conf['files_path'])) {
    drush_die(dt("Path to .htaccess does not exist.  Create it or provide another path.\n @path", array('@path' => $srv_conf['files_path'])));
  }
  
}

/**
 * @function _os_drush_confirm_file_overwrite($contents, $destination) {
 * 
 * Copies contents to a temp file.  Prompts user to preview, copy, or quit.  Only overwrites after user confirmation.
 */
function _os_drush_confirm_file_overwrite($contents, $destination) {
  list($filename) = array_reverse(explode('/', $destination));
  $temp = tempnam('/tmp', 'drush-odc-'.$filename.'-');
  file_put_contents($temp, $contents);
  
  
  $prompt_loop = TRUE;
  while ($prompt_loop) {
    $action = drush_prompt(dt('Created @filename file.  [v]iew, [d]iff, [c]opy, [s]skip it, or [q]uit?', array('@filename' => $filename)));
    switch ($action) {
      case 'v':
        drush_shell_exec_interactive('less ' . $temp);
        break;
  
      case 'd':
        drush_shell_exec_interactive("diff $destination $temp");
        break;
  
      case 'c':
        $prompt_loop = FALSE;
        break;
  
      case 'q':
        drush_die('User bailed out.');
        break;

      case 's':
        unlink($temp);
        return;
    }
  }
  // copy the .htaccess file.
  rename($destination, $destination . date('-old-U'));
  drush_print("copying $temp to $destination");
  copy($temp, $destination);
}

/**
 * @function _os_drush_domain_htaccess($multisites)
 * 
 * Builds .htaccess file for all multisites in array.
 */
function _os_drush_domain_htaccess($multisites) {
  $head =<<<EOF
# This .htaccess file automatically built by drush os-domain-config  
      
<IfModule mod_rewrite.c>
  RewriteEngine on

  # Note: this will redirect to the /sites subfolder which is identical to
  # the domain accessed by the browser.  If you use partial domain names as
  # the /sites subfolder (e.g. /sites/domain.com vs /sites/www.domain.com)
  # you will need to modify the Drupal root .htaccess to remove the leading www
  # or modify the following to use the protion of %{HTTP_HOST} that mirrors
  # your /sites subfolders

  
EOF;

  $foot =<<<EOF
  
  # If the domain does not have a dedicated /sites/<domain>/files folder allow the
  # default folder to catch it.
  #  RewriteCond %{REQUEST_FILENAME} !-f
  #  RewriteCond %{REQUEST_FILENAME} !-d
  # To redirect the URI to the actual location of the file /sites/default/files/<filename>
  # change the [L] to [L,R]
  #  RewriteRule ^(.*)$ /sites/default/files/$1 [L]

</IfModule>      

SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006
Options None                                                                                                                                                                                                                                   
Options +FollowSymLinks

EOF;
  
  $domain_rules = array();
  foreach ($multisites as $site => $domains) {
    $domain_rules[] = _os_drush_domain_htaccess_domain_rules($site, $domains);
  }
  
  return $head . implode("\n", $domain_rules) . $foot;
}

/**
 * @function _os_drush_domain_htaccess_domain_rules($site, $domains) 
 *
 * Generates htaccess rules for $site containing $domains[]
 */
function _os_drush_domain_htaccess_domain_rules($site, $domains) {
  $rules = <<<EOF
  
  RewriteCond %{REQUEST_FILENAME} !-f                                                                                  
  RewriteCond %{REQUEST_FILENAME} !-d 
  RewriteCond %{REQUEST_FILENAME} ^(.*?)\/files\/ 
EOF;
  
  $rules .= _os_drush_domain_htaccess_rewrite_condition($domains);
  $rules .= '  RewriteRule ^(.*)$ /sites/' . $site . '/files/$1 [L]' . "\n"; 
  return $rules;
}

/**
 * @function _os_drush_domain_htaccess_rewrite_condition($domains)
 * 
 * Given a list of domains, returns htaccess rewrite conditions matching all of them.
 */
function _os_drush_domain_htaccess_rewrite_condition($domains) {
  foreach ($domains as $n => $domain) {
    $condition = '  RewriteCond %{HTTP_POST} ^' . $domain;
    $domains[$n] = str_replace('.', '\.', $condition);
  }
  return "\n" . implode(" [OR]\n", $domains) . "\n";
}
