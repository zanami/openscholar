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
    'os-domain-config' => array(
      'description' => dt('Update Drupal configurations for a custom domain.'),
      'options' => array(
        'path-to-htaccess' => dt('Not implemented'),
        ''
      ),
      'arguments' => array(
        'domain' => dt('The domain to enable.  Do not include http://.'),
        'required' =>  TRUE,
          
      ),
      'examples' => array(
        'drush @prod odc example.com' => dt('Enable production site to use example.com domain.  The owner of example.com will still have to add that domain to her or her vsite.'),
        'drush @prod odc example.com --ac=yes' => dt('Also enables example.com domain.  When the ac option is enabled, the domain will be added to Acquia\'s configuration by way of the acquia cloud API.'),
      ),
        
      'aliases' => array('odc'),
    )
  );
}

/**
 * @function drush_os_domain_config
 *
 * Command callback for os-domain-config
 */
function drush_os_domain_config() {
  $args = func_get_args();
  
  if (!count($args)) {
    drush_die('drush os-domain-config requires a domain.  See drush odc --help');
  }
  
  $domain = str_replace('http://', '', $args[0]);
  $status = drush_sitealias_get_record('@self');
  $root = $status['root'];
  $context = drush_get_context();
  $base_path = $root . '/' . $context[DRUSH_DRUPAL_SITE_ROOT];
  $files_path = $root . '/files';
  
  //should this live elsewhere?  not sure acquia will let us keep the file where it is now.
  $domains_list = $base_path . '/' . 'domains.list';

  //make sure domain is valid and correct
  if (drush_prompt("Are you sure you want to add '$domain'? (y/n)", 'n') != 'y') {
    drush_die('User bailed out');
  }
  
  //check for domains.list
  if (!file_exists($domains_list)) {
    file_put_contents($domains_list, "# This file created automatically by drush os-domain-config.  Do not delete.\n") || drush_die('Error creating domains.list.  Check your permissions.');
    file_put_contents($domains_list, $status['uri'], FILE_APPEND);
  }
  
  //do we have sites.php?
  $sitesphp = $root . '/sites/sites.php';
  if (!file_exists($sitesphp)) {
    drush_die('sites.php is missing.  Please create a sites.php file to use os-domain-config.');
  }
  
  //check if sites.php has our include line
  $contents = file_get_contents($sitesphp);
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
  if (!is_dir($files_path)) {
    drush_die("Path to .htaccess does not exist.  Create it or provide another path.\n$files_path");
  }
  
  //append domain to list
  drush_file_append_data($domains_list, "\n".$domain); 
  
  
  //now lets build the files from out domains lists.
  
  //build list of multisites and their domains
  $sites = $root . '/sites/';
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
  $inc_file= $root . '/sites/sites.php.inc';
  file_put_contents($inc_file, "<?php\n\n\$sites = " . var_export($a_sites, TRUE) . ";\n");
  
  // htaccess
  $htaccess = _os_drush_domain_htaccess($multisites);
  $temp = tempnam('/tmp', 'drush-odc-');
  file_put_contents($temp, $htaccess);
  
  
  //would it make sense to diff the files and ask the user to confirm?
  $htaccess_file = "$files_path/.htaccess";
  $prompt_loop = TRUE;
  while ($prompt_loop) {
    $action = drush_prompt('Created .htaccess file.  [v]iew, [d]iff, [c]opy, or [a]bandon it?');
    drush_print($action);
    switch ($action) {
      case 'v':
        drush_shell_exec_interactive('less ' . $temp);
        break;
        
      case 'd':
        drush_shell_exec_interactive("diff $htaccess_file $temp");
        break;
        
      case 'c':
        $prompt_loop = FALSE;
        break;  
      
      case 'a': case 'q':
        drush_die('User bailed out.'); 
        break;
    }
  }
  // copy the .htaccess file.
  rename($htaccess_file, $htaccess_file . date('-ymd.U'));
  drush_print("copying $temp to $htaccess_file");
  copy($temp, $htaccess_file);
  
  // try acquia cloud api
  if (isset($context[DRUSH_COMMANDS]['ac-domain-add'])) {
    drush_invoke('ac-domain-add', array($domain));
    drush_invoke('ac-domain-purge', array($domain));    
  } 
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
    //$condition = '  RewriteCond %{HTTP_POST} (www.)?' . $domain;
    //begins with.  we have one for scholar.harvard, so it won't match openscholar.harvard.  shouldn't this always be the case?
    $condition = '  RewriteCond %{HTTP_POST} ^(www.)?' . $domain;
    $domains[$n] = str_replace('.', '\.', $condition);
  }
  
  return "\n" . implode(" [OR]\n", $domains) . "\n";
}
