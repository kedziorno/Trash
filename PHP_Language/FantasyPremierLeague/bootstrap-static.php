<?php

$mainurl = "https://fantasy.premierleague.com/api/";
$getUrls = array(
//	"bootstrap",
	"bootstrap-static",
//	"bootstrap-dynamic",
//	"events",
//	"elements",
//	"element-types",
//	"fixtures",
//	"teams",
//	"region"
);

$bootstrap_static = array(
	"phases",
	"elements",
	"game-settings",
	"total-players",
	"teams",
	"element_types",
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
		if($key === "phases") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}

		if($key === "elements") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
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
		if($key === "total-players") {
			fwrite($fp, $key."\n");
			fwrite($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "teams") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "element_types") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
		if($key === "events") {
			fwrite($fp, $key."\n");
			putArray($fp, $val);
			fwrite($fp, "\n\n");
		}
	}
	fclose($fp);
}

?>
