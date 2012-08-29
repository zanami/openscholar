<?php
/**
 * Function to check to see if a module exists
 *
 */
function harvard_activity_reports_check_module($moduleName){
	$module = $moduleName;
	//if(module_exists($module) ? true : false);
	if(module_exists($module)) {
		return TRUE;
	}else {
		return FALSE;
	}
}// end function
