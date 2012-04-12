<?php

class os_context_reaction_feature extends context_reaction {

  function execute() {
    return;
  	$context = current($this->get_contexts());
    
    //set the context
    context_set('os','feature',$context->xxxx);
  }
}
