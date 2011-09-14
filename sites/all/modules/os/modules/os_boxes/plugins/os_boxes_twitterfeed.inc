<?php

/**
 * @file
 * The box class for "Twitter feed" type widgets.
 */

class os_boxes_twitterfeed extends os_boxes_default {
	
	//Only cache twitter feeds for 15 minutes
	public $cache_max_lifespan = 900;
	
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    
    // the default value for the form
    $options['twitter_username'] = '';
    $options['followme_link'] = 0;
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    
    $form['twitter_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter username'),
      '#required' => TRUE,
      '#default_value' => $this->options['twitter_username'],
      '#description' => "To pull your Twitter posts into your site add your username to the above field (examples: '@username' says show all the tweets I have posted, 'from:username' says pull all the tweets and retweets I have sent, '#harvard_scholar' says pull anything tagged harvard_scholar)."
    );
    
    $form['followme_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Inlcude a "follow me link"'),
      '#default_value' => $this->options['followme_link']
    );

    $form += parent::options_form();
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    
		//if(!($vsite = vsite_get_vsite()) || !$vsite->access_feature($op = 'view', 'scholar_reader')) return array();
		
		// settings values
    $twitter_username = $this->options['twitter_username'];
    $followme_link = $this->options['followme_link'];


		$output = twitter_pull_render ($twitter_username);
		
		// include a followme link
		if ($this->options['twitter_followme'] !== 0) {
	      $output .= '<div class="twitter-followme">' . l('Follow me on Twitter','http://twitter.com/' . $twitter_username) .'</div>';
		}

    $block['content'] = $output;
    return $block;
  }

}