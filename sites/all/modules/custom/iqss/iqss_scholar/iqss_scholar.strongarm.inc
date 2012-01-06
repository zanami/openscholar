<?php

function iqss_scholar_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'addthis_config';
  $strongarm->value = array (
	  'services_toolbox' => '',
	  'services_compact' => '',
	  'services_expanded' => '',
	  'ui_cobrand' => '',
	  'ui_header_color' => '#000000',
	  'ui_header_background' => '#FFFFFF',
	  'ui_click' => 0,
	  'ui_delay' => '',
	  'ui_use_addressbook' => 0,
	  'pubid' => 'iqsssscholar',
	  'data_track_clickback' => true,
	  'data_ga_tracker' => 'pageTracker',
	  'ui_use_css' => true,
	  'data_use_cookies' => 0,
	);
  $export['addthis_config'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_title';
  $strongarm->value = 'RePEc';
  $export['citation_distribute_service_repec_title'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_inst_name';
  $strongarm->value = 'Harvard University OpenScholar';
  $export['citation_distribute_service_repec_inst_name'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_archive_code';
  $strongarm->value = 'qsh';
  $export['citation_distribute_service_repec_archive_code'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_maintainer_name';
  $strongarm->value = 'Jon Sagotsky';
  $export['citation_distribute_service_repec_maintainer_name'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_maintainer_email';
  $strongarm->value = 'jsagotsky@iq.harvard.edu';
  $export['citation_distribute_service_repec_maintainer_email'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_archive_path';
  $strongarm->value = '/nfs/www/edu-harvard-iq-repec/RePEc';
  $export['citation_distribute_service_repec_archive_path'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_archive_url';
  $strongarm->value = 'http://repec.iq.harvard.edu/RePEc/qsh';
  $export['citation_distribute_service_repec_archive_url'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_provider_institution';
  $strongarm->value = 'RePEc:edi:cbrssus';
  $export['citation_distribute_service_repec_provider_institution'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_repec_provider_homepage';
  $strongarm->value = 'http://scholar.harvard.edu';
  $export['citation_distribute_service_repec_provider_homepage'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'citation_distribute_service_googlescholar_auto_flag';
  $strongarm->value = TRUE;
  $export['citation_distribute_service_googlescholar_auto_flag'] = $strongarm;
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pinserver_pin_url';
  $strongarm->value = 'https://www.pin1.harvard.edu/pin/authenticate';

  $export['pinserver_pin_url'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pinserver_landing_path';
  $strongarm->value = 'site/register';

  $export['pinserver_landing_path'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pinserver_auth_str';
  $strongarm->value = 'Good signature from "authzproxy"';

  $export['pinserver_auth_str'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pinserver_support_contact';
  $strongarm->value = 'scholars_dev_support@help.hmdc.harvard.edu';

  $export['pinserver_support_contact'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pinserver_hash_huid';
  $strongarm->value = '0';

  $export['pinserver_hash_huid'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'iqss_remote_source_node_types';
  $strongarm->value = array('cv','scholar_software_project','page');

  $export['iqss_remote_source_node_types'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'iqss_remote_source_valid_ogs';
  $strongarm->value = '3633';

  $export['iqss_remote_source_valid_ogs'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'imageapi_jpeg_quality';
  $strongarm->value = 100;

  $export['imageapi_jpeg_quality'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'mollom_private_key';
  $strongarm->value = '32a73b2d09c7576f1a5d4ca73ca9dead';

  $export['mollom_private_key'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'mollom_public_key';
  $strongarm->value = 'fa9e0e813449535e5fcf06d6dda0d0f3';

  $export['mollom_public_key'] = $strongarm;
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'olark_code';
  $strongarm->value =<<<EOF
 <!-- begin olark code --><script type='text/javascript'>/*{literal}<![CDATA[*/window.olark||(function(i){var e=window,h=document,a=e.location.protocol=="https:"?"https:":"http:",g=i.name,b="load";(function(){e[g]=function(){(c.s=c.s||[]).push(arguments)};var c=e[g]._={},f=i.methods.length; while(f--){(function(j){e[g][j]=function(){e[g]("call",j,arguments)}})(i.methods[f])} c.l=i.loader;c.i=arguments.callee;c.f=setTimeout(function(){if(c.f){(new Image).src=a+"//"+c.l.replace(".js",".png")+"&"+escape(e.location.href)}c.f=null},20000);c.p={0:+new Date};c.P=function(j){c.p[j]=new Date-c.p[0]};function d(){c.P(b);e[g](b)}e.addEventListener?e.addEventListener(b,d,false):e.attachEvent("on"+b,d); (function(){function l(j){j="head";return["<",j,"></",j,"><",z,' onl'+'oad="var d=',B,";d.getElementsByTagName('head')[0].",y,"(d.",A,"('script')).",u,"='",a,"//",c.l,"'",'"',"></",z,">"].join("")}var z="body",s=h[z];if(!s){return setTimeout(arguments.callee,100)}c.P(1);var y="appendChild",A="createElement",u="src",r=h[A]("div"),G=r[y](h[A](g)),D=h[A]("iframe"),B="document",C="domain",q;r.style.display="none";s.insertBefore(r,s.firstChild).id=g;D.frameBorder="0";D.id=g+"-loader";if(/MSIE[ ]+6/.test(navigator.userAgent)){D.src="javascript:false"} D.allowTransparency="true";G[y](D);try{D.contentWindow[B].open()}catch(F){i[C]=h[C];q="javascript:var d="+B+".open();d.domain='"+h.domain+"';";D[u]=q+"void(0);"}try{var H=D.contentWindow[B];H.write(l());H.close()}catch(E){D[u]=q+'d.write("'+l().replace(/"/g,String.fromCharCode(92)+'"')+'");d.close();'}c.P(2)})()})()})({loader:(function(a){return "static.olark.com/jsclient/loader0.js?ts="+(a?a[1]:(+new Date))})(document.cookie.match(/olarkld=([0-9]+)/)),name:"olark",methods:["configure","extend","declare","identify"]});
/* custom configuration goes here (www.olark.com/documentation) */
olark.identify('8415-150-10-1872');/*]]>{/literal}*/</script>
<!-- end olark code --> 
EOF;
  $export['olark_code'] = $strongarm;  

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'olark_context';
  $strongarm->value = 1;
  $export['olark_code'] = $strongarm; 
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pinserver_app_name';
  $strongarm->value = '';

  $export['pinserver_app_name'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pinserver_gpg_dir';
  $strongarm->value = '';

  $export['pinserver_gpg_dir'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'error_level';
  $strongarm->value = error_reporting();

  $export['error_level'] = $strongarm;
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'shorten_bitly_login';
  $strongarm->value = 'openscholar';
  $export['shorten_bitly_login'] = $strongarm;
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'shorten_bitly_key';
  $strongarm->value = 'R_48a581b6939d4e0a117b961ba0c1bb5f';
  $export['shorten_bitly_key'] = $strongarm;
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'shorten_cache_fail_duration';
  $strongarm->value = '300';
  $export['shorten_cache_fail_duration'] = $strongarm;
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'shorten_service';
  $strongarm->value = 'j.mp';
  $export['shorten_service'] = $strongarm;
  
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'shorten_service_backup';
  $strongarm->value = 'tinyurl';
  $export['shorten_service_backup'] = $strongarm;

    
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'shorten_www';
  $strongarm->value = FALSE;
  $export['shorten_www'] = $strongarm;
  
  return $export;
}
