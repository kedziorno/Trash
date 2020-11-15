<?php

$userId = 0000;
$minPage = 1;
$maxPage = 2;
$totalPages = 0;

$urlLeaguesClassicStandings = "https://fantasy.premierleague.com/api/leagues-classic-standings/".$userId."?phase=1&le-page=1&ls-page=";

if($maxPage > $minPage)
    $totalPages = $maxPage - $minPage;
else
if($maxPage < $minPage)
    $totalPages = $minPage - $maxPage;

$lcs = array(
	"new_entries",
	"league",
	"standings",
	"update_status",
);

$myOutputFileName = "my-leagues-classic-standings";
$outputFileName = "leagues-classic-standings";

echo "Get leagues-classic-standings \n";

if(file_exists($myOutputFileName.".csv"))
    echo "***** use file my-leagues-classic-standings.csv *****\n";
else {
    $fp = fopen($outputFileName.".csv", 'w');

    $index = 0;
    $firstLine = true;
    for($index = $minPage; $index <= $maxPage; $index++) {
        $link = $urlLeaguesClassicStandings."".$index;
        echo "\t from URL $link ... \n";
        $jsonContent = file_get_contents($link, false, $context);
        $jsonObj = json_decode($jsonContent, true);
        foreach ($jsonObj as $key => $val) {
            if($key === "standings") {
                foreach ($val as $sKey => $sVal) {
                    if($sKey === "results") {
                        if($firstLine == true) {
                            putArray($fp,$sVal);
                            $firstLine = false;
                        } else {
                            putArrayWithoutHeader($fp,$sVal);
                        }
                    }
                }
            }
        }
    }

    fclose($fp);
}
