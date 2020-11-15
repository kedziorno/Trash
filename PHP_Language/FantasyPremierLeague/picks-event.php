<?php

$outputFileName = "leagues-classic-standings";
$delimiter = ",";
$myOutputFileName = "my-leagues-classic-standings";

//https://fantasy.premierleague.com/drf/entry/147481/event/1
//https://fantasy.premierleague.com/drf/entry/147481/event/1/picks

$urlEntry = "https://fantasy.premierleague.com/api/entry";

$gwG = 0;

// gameweek
echo "GameWeek from events.csv is ... ";
$evp = fopen("events.csv", "rb");
while (($data = fgetcsv($evp, 2048, $delimiter)) !== FALSE)  {
    $is_current = $data[12];
    //if($is_current == "") {
        //$gwG++;
    //}
    if($is_current == "1") {
        break;
    }
    $gwG++;
}
fclose($evp);

echo "$gwG\n";

if($gwG == 0) {
    echo "GameWeek is 0, exit !\n";
    exit(1);
}

// 21.05.2019
$gwparse = array();
$gwfd = fopen("GameWeek.txt","rt");
$gwcontent = fread($gwfd, 4096);
$gwarray = explode(",", $gwcontent);
foreach($gwarray as $gwitem) {
	if(is_numeric($gwitem) && $gwitem > 0) {
		$gwparse[] = $gwitem;
	} else {
		$gwitemarray = explode("-", $gwitem);
		$gwmin = $gwitemarray[0];
		if($gwmin <=0) continue;
		$gwmax = $gwitemarray[1];
		if($gwmax <=0) continue;
		for($index = $gwmin; $index <= $gwmax; $index++) {
			if($index <= $gwG)
				$gwparse[] = $index;
		}
	}
}
fclose($gwfd);

/* 
// event
$fp1 = fopen("event_leagues.csv", "w");
$fp2 = fopen("event_entry_history.csv", "w");
$fp3 = fopen("event_ce.csv", "w");
$fp4 = fopen("event_automatic_subs.csv", "w");
$fp5 = fopen("event_fixtures.csv", "w");
$fp6 = fopen("event_can_change_captain.csv", "w");
$fp7 = fopen("event_manager_subs.csv", "w");
$fp8 = fopen("event_picks.csv", "w");
$fp9 = fopen("event_own_entry.csv", "w");
$fp10 = fopen("event_state.csv", "w");
$fp11 = fopen("event_points.csv", "w");
$fp12 = fopen("event_entry.csv", "w");
$fp13 = fopen("event_active_chip.csv", "w");
*/

// picks
$fp1 = fopen("picks_active_chip.csv", "w");
$fp2 = fopen("picks_automatic_subs.csv", "w");
$fp3 = fopen("picks_entry_history.csv", "w");
$fp4 = fopen("picks_event.csv", "w");
$fp5 = fopen("picks_picks.csv", "w");

if(file_exists($myOutputFileName.".csv"))
    $uchwyt = fopen ($myOutputFileName.".csv","r"); // recznie edytowany plik my-leagues-classic-standings.csv
else
    $uchwyt = fopen ($outputFileName.".csv","r"); // plik nadpisywany za każdym razem

/*
// event
// headers
fwrite($fp1, "ID,GW,id,entry_rank,entry_last_rank,entry_movement,entry_change,entry_can_leave,entry_can_admin,entry_can_invite,entry_can_forum,entry_code,name,is_cup,short_name,created,closed,forum_disabled,make_code_public,rank,size,league_type,_scoring,ko_rounds,admin_entry,start_event\n");
//fwrite($fp2, "ID,GW,id,movement,points,total_points,rank,rank_sort,overall_rank,targets,event_transfers,event_transfers_cost,value,points_on_bench,bank,entry,event\n");
fwrite($fp3, "ID,GW,ce\n");
fwrite($fp4, "ID,GW,id,element_in,element_out,entry,event\n");
fwrite($fp5, "ID,GW,id,kickoff_time_formatted,started,event_day,deadline_time,deadline_time_formatted,stats,code,kickoff_time,team_h_score,team_a_score,finished,minutes,provisional_start_time,finished_provisional,event,team_a,team_h\n");
fwrite($fp6, "ID,GW,CAN CHANGE CAPTAIN\n");
fwrite($fp7, "ID,GW,manager_subs\n");
fwrite($fp8, "ID,GW,element,position,is_captain,is_vice_captain,can_sub,points,has_played,can_captain,explain,is_sub,element_type,stats,multiplier\n");
fwrite($fp9, "ID,GW,own_entry\n");
fwrite($fp10, "ID,GW,event,sub_state,event_day,deadline_time,deadline_time_formatted\n");
fwrite($fp11, "ID,GW,points\n");
//fwrite($fp12, "ID,GW,id,player_first_name,player_last_name,player_region_id,player_region_name,player_region_short_iso,summary_overall_points,summary_overall_rank,summary_event_points,summary_event_rank,joined_seconds,current_event,total_transfers,total_loans,total_loans_active,transfers_or_loans,deleted,email,joined_time,name,bank,value,kit,event_transfers,event_transfers_cost,extra_free_transfers,strategy,favourite_team,started_event,player\n");
fwrite($fp13, "ID,GW,active_chip\n");
*/
  
// picks
fwrite($fp1, "ID,GW,\n");
fwrite($fp2, "ID,GW,\n");
fwrite($fp3, "ID,GW,id,movement,points,total_points,rank,rank_sort,overall_rank,targets,event_transfers,event_transfers_cost,value,points_on_bench,bank,entry,event\n");
fwrite($fp4, "ID,GW,id,name,deadline_time,average_entry_score,finished,data_checked,highest_scoring_entry,deadline_time_epoch,deadline_time_game_offset,deadline_time_formatted,highest_score,is_previous,is_current,is_next,\n");
fwrite($fp5, "ID,GW,element,position,is_captain,is_vice_captain,multiplier,\n");

$row = 1;
if ($uchwyt !== FALSE) {
    while (($data = fgetcsv($uchwyt, 2048, $delimiter)) !== FALSE)  {
        if($row == 1) {
            $row++;
            continue;
        }
        
        $id = trim($data[10]);
        
        if(count($data) != 15) {
            echo "$row : line skipping - not equal 15 columns or line its empty\n";
            $row++;
            continue;
        }
        
        if($id == "") {
            echo "$row : line skipping because ID its empty\n";
            $row++;
            continue;
        }
        
        if(!intval($id) || !is_numeric($id)) {
            echo "$row : id not integer ($id)\n";
            $row++;
            continue;
        }
        
        echo "$row : parsing id $id\n";
        
        $gw = $gwG;
        
        foreach($gwparse as $gw) {
            //$urlEntryEvent = $urlEntry."/".$id."/event/".$gw;
            $urlEntryPicks = $urlEntry."/".$id."/event/".$gw."/picks";
            
            //echo "$id : get $urlEntryEvent\n";
            //fwrite($fpe, "$id : get $urlEntryEvent\n");
            //$jsonContent1 = file_get_contents($urlEntryEvent, false);
            echo "$id : get $urlEntryPicks\n";
            //fwrite($fpp, "$id : get $urlEntryPicks\n");
            $jsonContent2 = file_get_contents($urlEntryPicks, false);
            
            //echo "$id : json_decode $urlEntryEvent\n";
            //$jsonObj1 = json_decode($jsonContent1, true);
            echo "$id : json_decode $urlEntryPicks\n";
            $jsonObj2 = json_decode($jsonContent2, true);
            
            //print_r($jsonObj1);
            //print_r($jsonObj2);
            
            // event - niepotrzebne
            /*
            foreach ($jsonObj1 as $k1 => $v1) {
                if($k1 === "leagues") {
                    //fwrite($fpe, "* LEAGUES *,GW,$gw\n");
                    foreach($v1 as $ii=>$item) {
                        fwrite($fp1, $ii."\n");
                        if(count($item) == 0)
                            fwrite($fp1, "null\n");
                        else
                            putArrayAndKeysWithIdAndGW($fp1, $item, $id, $gw, false);
                    }
                    //fwrite($fpe, "\n\n");
		}
		if($k1 === "entry_history") {
                    //fwrite($fp2, "* ENTRY HISTORY *,GW,$gw\n");
                    putArrayLvl1($fp2, $id.",".$gw, $v1);
                    //fwrite($fp2, "\n\n");
		}
		if($k1 === "ce") {
                    //fwrite($fp3, "* CE *,GW,$gw\n");
                    fwrite($fp3, "$id,$gw,$v1\n");
                    //fwrite($fp3, "\n\n");
		}
		if($k1 === "automatic_subs") {
                    //fwrite($fp4, "* AUTOMATIC SUBS *,GW,$gw\n");
                    putArrayAndKeysWithIdAndGW($fp4, $v1, $id, $gw, false);
                    //fwrite($fp4, "\n");
		}
		if($k1 === "fixtures") {
                    //fwrite($fp5, "* FIXTURES *,GW,$gw\n");
                    putArrayAndKeysWithIdAndGW($fp5, $v1, $id, $gw, false);
                    //fwrite($fp5, "\n\n");
		}
		if($k1 === "can_change_captain") {
                    //fwrite($fp6, "* CAN CHANGE CAPTAIN *,GW,$gw\n");
                    fwrite($fp6, "$id,$gw,$v1\n");
                    //fwrite($fp6, "\n\n");
		}
		if($k1 === "manager_subs") {
                    //fwrite($fp7, "* MANAGER SUBS *,GW,$gw\n");
                    putArrayAndKeysWithIdAndGW($fp7, $v1, $id, $gw, false);
                    //fwrite($fp7, "\n\n");
		}
		if($k1 === "picks") {
                    //fwrite($fp8, "* PICKS *,GW,$gw\n");
                    putArrayAndKeysWithIdAndGW($fp8, $v1, $id, $gw, false);
                    //fwrite($fp8, "\n\n");
		}
		if($k1 === "own_entry") {
                    //fwrite($fp9, "* OWN ENTRY *,GW,$gw\n");
                    fwrite($fp9, "$id,$gw,$v1\n");
                    //fwrite($fp9, "\n\n");
		}
		if($k1 === "state") {
                    //fwrite($fp10, "* STATE *,GW,$gw\n");
                    putArrayLvl1($fp10, $id.",".$gw, $v1);
                    //fwrite($fp10, "\n\n");
		}
		if($k1 === "points") {
                    //fwrite($fp11, "* POINTS *,GW,$gw\n");
                    fwrite($fp11, "$id,$gw,$v1\n");
                    //fwrite($fp11, "\n\n");
		}
		if($k1 === "entry") {
                    //fwrite($fp12, "* ENTRY *,GW,$gw\n");
                    putArrayLvl1($fp12, $id.",".$gw, $v1);
                    //fwrite($fp12, "\n\n");
		}
		if($k1 === "active_chip") {
                    //fwrite($fp13, "* ACTIVE CHIP *,GW,$gw\n");
                    fwrite($fp13, "$id,$gw,$v1\n");
                    //fwrite($fp13, "\n\n");
		}
            }
            */
            
            // picks
            foreach ($jsonObj2 as $k1 => $v1) {
                if($k1 === "active_chip") {
                    //fwrite($fp1, "* ACTIVE CHIP *,GW,$gw\n");
                    fwrite($fp1, "$id,$gw,$v1\n");
                    //fwrite($fp1, "\n\n");
		}
		if($k1 === "automatic_subs") {
                    //fwrite($fp2, "* AUTOMATIC SUBS *,GW,$gw\n");
                    putArrayAndKeysWithIdAndGW($fp2, $v1, $id, $gw, false);
                    //fwrite($fp2, "\n\n");
		}
		if($k1 === "entry_history") {
                    //fwrite($fp3, "* ENTRY HISTORY *,GW,$gw\n");
                    putArrayLvl1($fp3, $id.",".$gw, $v1);
                    //fwrite($fp3, "\n\n");
		}
		if($k1 === "event") {
                    //fwrite($fp4, "* EVENT *,GW,$gw\n");
                    putArrayLvl1($fp4, $id.",".$gw, $v1);
                    //fwrite($fp4, "\n\n");
		}
		if($k1 === "picks") {
                    //fwrite($fp5, "* PICKS *,GW,$gw\n");
                    putArrayAndKeysWithIdAndGW($fp5, $v1, $id, $gw, false);
                    //fwrite($fp5, "\n\n");
		}
            }
        }
        $row++;
    }
    fclose ($uchwyt);
}

//fclose($fpp);

//fclose($fp13);
//fclose($fp12);
//fclose($fp11);
//fclose($fp10);
//fclose($fp9);
//fclose($fp8);
//fclose($fp7);
//fclose($fp6);
fclose($fp5);
fclose($fp4);
fclose($fp3);
fclose($fp2);
fclose($fp1);

?>
