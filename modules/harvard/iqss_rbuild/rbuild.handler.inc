<?php
/**
 * @file
 * 
 */

/**
 * 
 */
require_once(drupal_get_path('module', 'os_software') . "/package_handlers/scholar_software_handler.inc");

/**
 * Implements OS Software handler for Rbuild.
 * @author oren [or] <oren_robinson@harvard.edu>
 *
 */
class rbuild_handler implements os_software_handler {
  
	private static $cached_packages = array();
	
  private $packages = array();  // An array of associative arrays of releases
  private $short_name = FALSE;  // Unique machine name for this Rbuild product
  private $url = FALSE;         // The remote repository URL

  /**
   * Constructor
   * @param string $url
   * @param string $app_key
   */
  function __construct($url, $short_name = FALSE) {
  	
    // Caches all projects
  	if ($url) {
  	  $new_url = $this->create_url($url);
  	  if (!array_key_exists($new_url, self::$cached_packages)) {
  		  self::$cached_packages[$new_url] = $this->rbuild_parse($new_url);
  	  }
  	}
  	
  	// Uses cached packages or tries this project directly
  	if(count(self::$cached_packages[$this->create_url($url)])){
  		$this->packages = self::$cached_packages[$this->create_url($url)];
  	} elseif ($url) {
  		$this->packages = $this->rbuild_parse($this->create_url($url, $short_name));
  	}
  	
    $this->short_name = $short_name;
    $this->url = rtrim($url, '/ ');
    
    return count($this->packages) ? TRUE : FALSE;
  }
  
  /**
   * Public functions
   */
  
  /**
   * Gets all packages
   * 
   * @return array $packages
   */
  public function get_packages() {
    return $this->packages;
  }

  /**
   * Gets the package if App Key is defined.
   * 
   * @return mixed $package
   *   Returns array if exists, otherwise returns FALSE.
   */
  public function get_package() {
  	if (!$this->short_name) return FALSE;
  	if (!array_key_exists($this->short_name, $this->packages)) return FALSE;
  	
    return $this->packages[$this->short_name];
  }
  
  /**
   * Alters a project node before it is saved
   * @param object $project_node
   */
  public function alter_project(&$project_node) {
    $changed = FALSE;
  	if(!($package = $this->get_package())) return $changed;
  	
  	// Updates the title with what is in rbuild
  	if(!strlen($project_node->title)) {
  	  $short_name_stored = substr($package['title'], 0, strlen($this->short_name));
  	  $is_new_key = (strtolower($short_name_stored) != strtolower($this->short_name));
	  	if (strlen($this->short_name) && $is_new_key) {
	  	  $short_name = "$this->short_name: ";
	  	} else {
	  	  $short_name = '';
	  	}
	  	
	  	$project_node->title = $short_name . $package['title'];
	  	$changed = TRUE;
  	}
  	
  	/**
  	 * @todo do we still need this? [or] 2012-08-21
  	 *
  	if(!strlen($project_node->body)) {
  	  // Checks for remote docs
	  	$docs_source_path = $this->docs_source_path($this->short_name, $package['version']);
	  	$body_headers = _rbuild_get_file_headers($docs_source_path);
	  	
	  	// Adds any new documentation to the Software Project node.
	  	if (!module_exists('iqss_remote_source') || strstr($body_headers[0], '404 Not Found')) {
	  	  $body_content = '<!-- RBuild Docs -->';
		  	if (strlen($package['author'])) {
		  	  $body_content .= ''
		  	    . 'Contributors: '
		  	    . strip_tags(trim($package['author'], ' ,'))
		  	    . '<br /><br />';
		  	}
		  	$body_content .= $package['description'];
		  	$project_node->body = $body_content;
        $changed = TRUE;
	  	}
	  	
	  	else if ($project_node->iqss_remote_source_path != $docs_source_path) {
	      $project_node->iqss_remote_source_path = $docs_source_path;
	  	  $changed = TRUE;
	  	}
  	}
  	 */
  	
  	return $changed;
  }
  
  /**
   * Creates the initial batch of release files
   * @param $project_node
   */
  public function init_releases($project_node) {
  	if (!$project_node->is_new || !$project_node->nid) return;
  	
  	$releases = $this->rbuild_get_release_files(); // array
  	$package  = $this->get_package();              // array
  	
  	$operations = array();
  	foreach ($releases as $version => $filename){
  		// Creates new Software Release node
  		$operations[] = array(
  			'_rbuild_add_release_node',
  		  array($project_node, $version, $filename, $package, $this->url),
  		);
  	}
  	
  	$batch = array(
			'title' => t('Importing'),
			'operations' => $operations,
			'finished' => 'iqss_rbuild_batch_finished',
		);
		
  	batch_set($batch);
  }
  
  /**
   * Checks that rBuild short name is found at URL
   */
  public function validate() {
    if(!($package = $this->get_package())) {
      form_set_error('rbuild identifier', "Invalid rBuild short name [$this->short_name] not found at [$this->url]");
    }
  }
  
  /**
   * Proccesses cron job to check for updates.
   */
  public function find_new_releases(&$project_node, $fetch_releases = FALSE) {
    
    // Only proceeds if we have a package
    if(!($package = $this->get_package())) return FALSE;
    
    // Updates title and description
    $changed = $this->alter_project($project_node);
    
    // Checks to see if latest release already exists as a node.
    $project_nid = $project_node->nid;
    $version = $package['version'];
    $release_node_exists = _rbuild_query_release_node_exists($project_nid, $version);
    
    // Exits without making changes if the release already exists
    if ($release_node_exists === TRUE) return FALSE;
    
    // Adds this new Software Release node then returns TRUE
   	$filename = $this->short_name . '_' . $package['version'] . '.tar.gz';
  	_rbuild_add_release_node($project_node, $package['version'], $filename, $package, $this->url);
  	
    return TRUE;
  }

  /**
   * Private functions
   */
  
  /**
   * Creates URL path.
   *
   * @param string $s_url
   * @param string $s_app_key
   * @return string
   */
  private function create_url($url, $short_name = FALSE) {
  	$path = rtrim($url, '/ ') . '/PACKAGES';
  	
  	// Appends rBuild short name if given
    if (strlen($short_name) > 0) $path .= $short_name;
    
    return $path;
  }
  
  /**
   * Parses a package file and retrieves avalible packages
   * @param $url
   * @return array $packages
   */
  private function rbuild_parse($url) {
    $package_file = _rbuild_get_file_contents($url);
    
    $packages_raw = split("\n\n\n", $package_file);
    
    // Prepares package name and information.
    $packages = array();
    foreach ($packages_raw as $package_raw) {
    	$package_info = array();
    	$package_name = FALSE;
    	$package_raw = trim($package_raw);
      $lines = split("\n", $package_raw);
      
      // Searches lines like "Key: Value" for package name and metadata.
      foreach ($lines as $line) {
        $line_elements = (explode(':', $line, 2));
        if (count($line_elements) == 2) {
          $current_key = strtolower(trim($line_elements[0]));
          $current_value = trim($line_elements[1]);
          if ($current_key === 'package') {
            $package_name = $current_value;
          }
          $package_info[$current_key] = $current_value;
        } else if ($current_key) {
          $package_info[$current_key] .= ' ' . trim($line_elements[0]);
        }
      }
      
      if ($package_name && count($package_info)) {
      	$packages[$package_name] = $package_info;
      }
    }
    
    return $packages;
  }
  
  /**
   * Parses a package's files.
   * 
   * @param $url
   * @return array $releases
   *   An array of strings containing release package filepaths
   */
  private function rbuild_get_release_files() {
  	
  	$releases = array();
  	$index_file_data = _rbuild_get_file_contents($this->url . '/');
	  if (strlen($index_file_data)) {
	  	$matches = array();
	  	$pattern = '/(a href\=")(' . $this->short_name . '([\._-\d]*)\.tar\.gz)(")/i';
	  	preg_match_all($pattern, $index_file_data, $matches);
	    foreach ($matches[2] as $index => $filename) {
	    	$releases[trim($matches[3][$index], '_')] = $filename;
	    }
	  }
    
	  if (!count($releases)) {
	  	$package = $this->get_package();
	  	$default_filepath = $this->url . '/' . $this->short_name . '_' . $package['version'] . '.tar.gz';
	  	if (file_exists($default_filepath)) {
	  	  $releases[$package['version']] = $default_filepath;
	  	}
	  }
	  
	  return $releases;
  }
  
  /**
   * Formats a documentation URL
   * 
   * @param string $short_name
   * @param string $version
   * 
   * @return string $url
   *
   * @todo still needed?
   * @see rbuild_package_handler::alter_project()
   * 
  public function docs_source_path($short_name, $version) {
    $url_parts = array(
      'http://r.iq.harvard.edu/docs/',
      strtolower($short_name),
      '/',
      $version,
      '/index.shtml',
    );
    
    $url = implode('', $url_parts);
    return $url;
  }
   */
}