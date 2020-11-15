<?php

$urlBootstrapStatic = "https://fantasy.premierleague.com/api/bootstrap-static";
$urlElementSummary = "https://fantasy.premierleague.com/api/element-summary";

$bootstrap_static = array(
	"phases",
	"elements",
	"game-settings",
	"total-players",
	"teams",
	"element_types",
	"events",
);

$outputFileName = "element-summary";

$element_summary = array(
	"history_past",
	"fixtures_summary",
	"explain",
	"history_summary",
	"fixtures",
	"history"
);

echo "Get BOOTSTRAP_STATIC from URL $urlBootstrapStatic ... \n";
$jsonContent = file_get_contents($urlBootstrapStatic, false, $context);
$jsonObj = json_decode($jsonContent, true);

$fp = fopen("element-summary.csv", 'w');

$fp1 = fopen($outputFileName."-".$element_summary[0].".csv", "w");
$fp2 = fopen($outputFileName."-".$element_summary[1].".csv", "w");
$fp3 = fopen($outputFileName."-".$element_summary[2].".csv", "w");
$fp4 = fopen($outputFileName."-".$element_summary[3].".csv", "w");
$fp5 = fopen($outputFileName."-".$element_summary[4].".csv", "w");
$fp6 = fopen($outputFileName."-".$element_summary[5].".csv", "w");

foreach ($jsonObj as $key => $val) {
	if($key === "elements") {
		$boolPutKeys = true;
		foreach($val as $valObject) {
			fwrite($fp, $key."\n");
			$id = $valObject['id'];
			$urlElementSummaryForId = $urlElementSummary."/".$id;
			echo "Request for $urlElementSummaryForId ... \n";
			$jsonContentElementSummary = file_get_contents($urlElementSummaryForId, false, $context);
			echo "Parse JSON for $urlElementSummaryForId ... \n";
			$jsonContentForEs = json_decode($jsonContentElementSummary, true);
			fwrite($fp, $urlElementSummaryForId);
			fwrite($fp, "\n");
			foreach($jsonContentForEs as $esKey => $esVal) {
				if($esKey === "history_past") {
					echo "\t *** ".$esKey."\n";
					fwrite($fp, $esKey."\n");
					putArrayAndKeysWithId($fp1, $esVal, $id, $boolPutKeys);
				}
				if($esKey === "fixtures_summary") {
					echo "\t *** ".$esKey."\n";
					fwrite($fp, $esKey."\n");
					putArrayAndKeysWithId($fp2, $esVal, $id, $boolPutKeys);
				}
				if($esKey === "explain") {
					echo "\t *** ".$esKey."\n";
					fwrite($fp, $esKey."\n");
					$explainObject = $esVal[0];
					foreach($explainObject as $esK1 => $esV1) {
						if($esK1 === "explain") {
							putArrayLvl1($fp3, "minutes", $esV1['minutes']);
						}
						if($esK1 === "fixture") {
							putArrayLvl1($fp3, "fixture", $esV1);
						}
					}
					fwrite($fp, "\n");
				}
				if($esKey === "history_summary") {
					echo "\t *** ".$esKey."\n";
					fwrite($fp, $esKey."\n");
					putArrayAndKeysWithId($fp4, $esVal, $id, $boolPutKeys);
				}
				if($esKey === "fixtures") {
					echo "\t *** ".$esKey."\n";
					fwrite($fp, $esKey."\n");
					putArrayAndKeysWithId($fp5, $esVal, $id, $boolPutKeys);
				}
				if($esKey === "history") {
					echo "\t *** ".$esKey."\n";
					fwrite($fp, $esKey."\n");
					putArrayAndKeysWithId($fp6, $esVal, $id, $boolPutKeys);
					$boolPutKeys = false;
				}
			}
			fwrite($fp, "\n\n");
		}
	}
}

fclose($fp);

fclose($fp6);
fclose($fp5);
fclose($fp4);
fclose($fp3);
fclose($fp2);
fclose($fp1);

?>
