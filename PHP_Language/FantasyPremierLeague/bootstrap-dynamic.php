<?php

$mainurl = "https://fantasy.premierleague.com/api/";
$getUrls = array(
//	"bootstrap",
//	"bootstrap-static",
	"bootstrap-dynamic",
//	"events",
//	"elements",
//	"element-types",
//	"fixtures",
//	"teams",
//	"region"
);

$bootstrap_dynamic = array(
	"stats",
	"current-event",
	"player",
	"stats_options",
	"last-entry-event",
	"watched",
	"entry",
	"next_event_fixtures",
	"next-event"
);

foreach($getUrls as $gu) {
	echo "*** ".$gu."\n";

	if(true) {
		echo "From URL...\n";
		$jsonContent = file_get_contents($mainurl.$gu, false, $context);
		$fp = fopen($gu.".json", 'wt');
		fwrite($fp, $jsonContent);
		fclose($fp);
	} else {
		$jsonContent = file_get_contents($gu.".json");
		echo "From FILE...";
	}
	$jsonObj = json_decode($jsonContent, true);
	
	$fp = fopen($gu.".csv", 'w');
		
	foreach ($jsonObj as $key=>$val) {
		if($key === "stats") {
			// headings
			fwrite($fp, "stats headings\n");
			putArray($fp, $val['headings']);
			fwrite($fp, "\n\n");
			// headings
			fwrite($fp, "stats categories\n");
			putArray($fp, $val['categories']);
			fwrite($fp, "\n\n");
		}
		if($key === "current-event") {
			fwrite($fp, $key."\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "player") {
			fwrite($fp, $key."\n");
			putArrayLvl1($fp, "", $val);
			fwrite($fp, "\n\n");
		}
		if($key === "stats_options") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "last-entry-event") {
			fwrite($fp, $key."\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "watched") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "entry") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "next_event_fixtures") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "next-event") {
			fwrite($fp, $key."\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}
	}
	fclose($fp);
}

?>
