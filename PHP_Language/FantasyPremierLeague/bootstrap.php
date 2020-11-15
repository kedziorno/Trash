<?php

$mainurl = "https://fantasy.premierleague.com/api/";
$getUrls = array(
	"bootstrap",
//	"bootstrap-static",
//	"bootstrap-dynamic",
//	"events",
//	"elements",
//	"element-types",
//	"fixtures",
//	"teams",
//	"region"
);

$bootstrap = array(
	"elements",
	"total-players",
	"player",
	"element_types",
	"watched",
	"next-event",
	"phases",
	"stats",
	"game-settings",
	"current-event",
	"teams",
	"stats_options",
	"last-entry-event",
	"entry",
	"next_event_fixtures",
	"events",
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
		
	foreach ($jsonObj as $key=>$val) {
		//total-players
		if($key === "total-players") {
			fwrite($fp, "total-players\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}

		if($key === "elements") {
			fwrite($fp, "elements\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "player") {
			fwrite($fp, "player\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "element_types") {
			fwrite($fp, "element_types\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "watched") {
			fwrite($fp, "watched\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "next-event") {
			fwrite($fp, "next-event\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "phases") {
			fwrite($fp, "phases\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
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
		if($key === "game-settings") {
			// game
			fwrite($fp, "game-settings game\n");
			foreach($val['game'] as $gk=>$gv) {
				if($gk === "formations") {
					putArrayLvl3($fp, $gk, $gv);
				} else
				if($gk === "default_formation") {
					putArrayLvl2($fp, $gk, $gv);
				} else
				fwrite($fp, $gk.",".$gv."\n");
			}
			fwrite($fp, "\n\n");
			//element_type
			fwrite($fp, "game-settings element_type\n");
			putArray($fp,$val['element_type']);
			fwrite($fp, "\n\n");
		}
		if($key === "current-event") {
			fwrite($fp, "current-event\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "teams") {
			fwrite($fp, "teams\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "stats_options") {
			fwrite($fp, "stats_options\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "last-entry-event") {
			fwrite($fp, "last-entry-event\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "entry") {
			fwrite($fp, "entry\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "next_event_fixtures") {
			fwrite($fp, "next_event_fixtures\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "events") {
			fwrite($fp, "events\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
	}
	
	fclose($fp);
}

?>
