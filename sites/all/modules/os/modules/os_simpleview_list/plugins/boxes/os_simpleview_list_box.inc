<?php 

/**
 * @class os_simpleview_list
 * 
 * Box for displaying/configuring os_simpleview_lists 
 */

//class os_simpleview_list_box extends os_boxes_modalframe {
class os_simpleview_list_box extends os_boxes_default {
  
  final public function get_config_path() {
     // return $this->config_path().'/modal/box/'.($this->delta?$this->delta:$this->plugin_key);
     return 'os_simplview_list/nojs/config_path_yay!';
  }
/*  protected function config_path() {
    $path = 'os_simpleview_list/nojs';
    return $path;
  }*/
  
  public function options_defaults() {
    $options = parent::options_defaults();
    //need to know $class in order to get defaults from svlist plugin
    $options += $this->_get_list_class()->options_defaults(); 
    
    return $options;
  }
  
  /* in this case options_form only applies to box conf */
  public function options_form() {
    
    //if we're editing the page , give them the form page.  ctools_render should take over before this can return
    if ($_POST['js']) {
      return os_simpleview_list_page('js', NULL, $this->delta);
    } else {
      //otherwise we're just checking that a form exists so give one of those.
      return parent::options_form();
    }
    
//    $content_type = ($this->options['content_type']) ? $this->options['content_type'] : 'default';
  //  $form = $this->_get_list_class()->os_simpleview_list_form_basics();
//    return $form;
  }
  
  
  public function render() {
    $block = parent::render();
    
    $plugin_output = $this->_get_list_class()->render($this->options + array('delta' => $this->delta));
    
    //results will be page string or block array.  handle both.
    $block['content'] = (is_array($plugin_output)) ? $plugin_output['content'] : $plugin_output; 
    
    
    return $block;
  }
  
  /**
   * Called from views_pre_view
	 * Configures view according to options
   */
  public function views_alter(&$view, &$display_id, &$args) {
    //Use content type's class's views_alter.
    $instance = $this->_get_list_class();
    
    //merge in plugin opts.  more convenient to pass along this way, but that may chagne when we have to save edited opts. //@todo
    $instance->views_alter($view, $display_id, $args, array_merge($this->options, $this->options['plugin_opts']));
  }
  
  function _get_list_class() {
    //$class_name = ($this->options['content_type'] == 'all') ? 'default' : $this->options['content_type'];
    
    $plugins = os_simpleview_list_plugin_load();
    
    if ($this->options['content_type']) {
      foreach ($plugins as $key => $def) {
        if ($def['content_type'] == $this->options['content_type']) {
          $plugin_key = $key;
          break;
        }
      }  
    }
    //this->options is ignoring content_type in pubs...
    $plugin = (isset($plugin_key)) ? $plugins[$plugin_key] : $plugins['default'];
    
    $class = ctools_plugin_get_class($plugin, 'handler');
    return ($class) ? new $class() : FALSE;
  }
  
  //parent::save();
}
