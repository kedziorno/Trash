<?php
//ini_set('error_reporting', E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED & ~E_WARNING);
include('includes.php');

// v1
/*
require('bootstrap.php');
require('bootstrap-dynamic.php');
require('bootstrap-static.php');
require('elements.php');
require('element-types.php');
require('events.php');
require('fixtures.php');
require('region.php');
require('teams.php');
require('element-summary.php');
*/

// v2

require('events.php'); // GameWeek
require('leagues-classic-standings.php');
require('picks-event.php');

?>