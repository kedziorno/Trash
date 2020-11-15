<?php

$mainurl = "https://fantasy.premierleague.com/api/";
$getUrls = array(
//	"bootstrap",
//	"bootstrap-static",
//	"bootstrap-dynamic",
//	"events",
//	"elements",
//	"element-types",
//	"fixtures",
//	"teams",
	"region"
);

$region = array(
	"region",
);

foreach($getUrls as $gu) {
	echo "*** ".$gu."\n";

	if(true) {
		echo "From URL...\n";
		$jsonContent = file_get_contents($mainurl.$gu,false,$context);
		$fp = fopen($gu.".json", 'wt');
		fwrite($fp, $jsonContent);
		fclose($fp);
	} else {
		$jsonContent = file_get_contents($gu.".json");
		echo "From FILE...";
	}
	$jsonObj = json_decode($jsonContent, true);
	
	$fp = fopen($gu.".csv", 'w');
	
	putArray($fp, $jsonObj);
	
	fclose($fp);
}

?>
