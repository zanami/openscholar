<?php

/**
 * @file
 *
 * class for the stat counter box
 */


class os_boxes_statcounter extends os_boxes_default {
  
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['sc_project']  = 0;
    $options['sc_security'] = '';
    $options['show_title'] = TRUE;
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){

    $form = array();
    
    $form['readme'] = array(
      '#type' => 'markup',
      '#value' => '<div id="#statcounter_readme" class="messages">'.t('To use this box you will need to set up a StatCounter.com project.  Look at the default install guide and copy the sc_project and sc_security values into the forms below.').'</div>',
    );
    
    $form['show_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show Title'),
      '#description' => t('Enable this option to show the title of this box when displaying the box contents.'),
      '#default_value' => $this->options['show_title'],
    );
    
    $form['sc_project'] = array(
      '#type' => 'textfield',
      '#title' => t('Project'),
      '#description' => t('The value of "sc_project" from the StatCounter HTML code block for your project.  (e.g. 7338219)'),
      '#default_value' => $this->options['sc_project'],
    );

    $form['sc_security'] = array(
      '#type' => 'textfield',
      '#title' => t('Security Key'),
      '#description' => t('The value of "sc_security" from the StatCounter HTML code block for your project.  (e.g. daf3c75e)'),
    	'#default_value' => $this->options['sc_security'],
    );
    
    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $code_block = <<<EOF
<!-- Start of StatCounter Code for Default Guide -->
<script type="text/javascript">
var sc_project=%s; 
var sc_invisible=0; 
var sc_security="%s"; 
</script>
<script type="text/javascript"
src="http://www.statcounter.com/counter/counter.js"></script>
<noscript><div class="statcounter"><a title="tumblr hit
counter" href="http://statcounter.com/tumblr/"
target="_blank"><img class="statcounter"
src="http://c.statcounter.com/%s/0/%s/0/"
alt="tumblr hit counter"></a></div></noscript>
<!-- End of StatCounter Code for Default Guide -->    
EOF;

    $project  = check_plain($this->options['sc_project']);
    $security = check_plain($this->options['sc_security']);
    
    $block = parent::render();
    $block['content'] = sprintf($code_block, $project, $security, $project, $security);
    $block['subject'] = ($this->options['show_title']) ? $block['subject'] : '';

    return $block;
  }

}
 