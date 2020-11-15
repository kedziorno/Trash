<?php

$username = 'XXXXXXXXXXXXXXXXXX@fantasypl.pl';
$password = 'XXXXXXXXXXXXXX';

$context = stream_context_create(
array(
		'http'=>array(
			'header'  => "Authorization: Basic " . base64_encode("$username:$password")
		),
		"ssl"=>array(
            "verify_peer"=>false,
            "verify_peer_name"=>false,
        ),
));

// https://i-mscp.net/index.php/Thread/4208-Problem-with-PHP-function-file-get-contents/
function checkWrappers() {
	$w = stream_get_wrappers();
	echo 'openssl: ';echo extension_loaded ('openssl') ? 'yes':'no'; echo "\n";
	echo 'http wrapper: '; echo in_array('http', $w) ? 'yes':'no'; echo "\n";
	echo 'https wrapper: '; echo in_array('https', $w) ? 'yes':'no'; echo "\n";
	echo 'wrappers: '; var_dump($w);
	die();
}

// http://stackoverflow.com/a/12446906
function getSslPage($url) {
	$username = 'XXXXXXXXXXXXXXXXXX@fantasypl.pl';
	$password = 'XXXXXXXXXXXXXX';
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_USERPWD, $username . ":" . $password); 
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_setopt($ch, CURLOPT_HEADER, false);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_REFERER, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
	$result = curl_exec($ch);
	curl_close($ch);
	return $result;
}

//$result = file_get_contents('https://fantasy.premierleague.com/api/leagues-classic/1827/standings/?page_new_entries=1&page_standings=1', false, $context);

$result = getSslPage('https://fantasy.premierleague.com/accounts/login/');
echo $result;
$result = getSslPage('https://fantasy.premierleague.com/api/leagues-classic/1827/standings/?page_new_entries=1&page_standings=1');
echo $result;

die();

function putArray($fp, $val) { 
        $firstLine = true; 
        foreach($val as $k1=>$v1) { 
                $header = ""; 
                $values = ""; 
                foreach($v1 as $k2 => $v2) { 
                        if($firstLine) { 
                                $header .= "$k2;"; 
                        } 
                        $values .= "$v2;"; 
                } 
                if($firstLine) { 
                        fputcsv($fp, explode(";",$header)); 
                        $firstLine = false; 
                } 
                fputcsv($fp, explode(";",$values)); 
                $header = ""; 
                $values = ""; 
        } 
} 

function putArrayAndKeysWithId($fp, $val, $id, $putKeys = true) { 
        $firstLine = true; 
        foreach($val as $k1=>$v1) { 
                $header = "ID;"; 
                $values = "$id;"; 
                foreach($v1 as $k2 => $v2) { 
                        if($firstLine) { 
                                $header .= "$k2;"; 
                        } 
                        $values .= "$v2;"; 
                } 
                if($firstLine) { 
                        if($putKeys == true)
							fputcsv($fp, explode(";",$header)); 
                        $firstLine = false; 
                } 
                fputcsv($fp, explode(";",$values)); 
                $header = ""; 
                $values = ""; 
        } 
}

function putArrayLvl1($fp, $gk, $gv) {
	if(count($gv)) {
		foreach($gv as $k=>$v) {
			fwrite($fp, $gk.",".$k.",".$v."\n"); 
		}
	} else 
		fwrite($fp, "\n");
}

function putArrayLvl2($fp, $gk, $gv) { 
        foreach($gv as $gv1=>$gv2) { 
                foreach($gv2 as $gv21=>$gv22)  
                        fwrite($fp, $gk.",".$gv1.",".$gv21.",".$gv22."\n"); 
        } 
} 
 
function putArrayLvl3($fp, $gk, $gv) { 
        foreach($gv as $gv1=>$gv2) { 
                foreach($gv2 as $gv21=>$gv22) { 
                        foreach($gv22 as $gv221=>$gv222) { 
                                fwrite($fp, $gk.",".$gv1.",".$gv21.",".$gv221.",".$gv222."\n"); 
                        } 
                } 
        } 
} 

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

?>
