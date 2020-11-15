<?php
$id_c = mysql_connect("localhost", "koblak85_test", "test13132");
mysql_select_db("koblak85_test", $id_c);

// Ver 1
list($y,$m,$d) = explode(":", date("Y:n:j"));
mysql_query("START TRANSACTION", $id_c);
mysql_query("INSERT INTO visit(year,month,day,addr_ip) VALUES('$y','$m','$d','".$_SERVER['REMOTE_ADDR']."')",$id_c);
mysql_query("COMMIT", $id_c);

if(isset($_GET['GET_CV']))
{
	header('Cache-Control: public');
        header('Content-Type: application/pdf');
        header('Content-Disposition: filename="CV_inf.pdf"');
        readfile("CV_inf.pdf");
        die();
} 
else 
if(isset($_GET['EASY_CV']))
{
	header('Cache-Control: public');
        header('Content-Type: application/pdf');
        header('Content-Disposition: filename="CV_zwykle.pdf"');
        readfile("CV_zwykle.pdf");
        die();
} 
else 
{
//Ver 2
//mysql_query("START TRANSACTION", $id_conn);
//mysql_query("INSERT INTO visit(date,addr_ip) VALUES(CURRENT_DATE(),'".$_SERVER['REMOTE_ADDR']."')",$id_conn);
//mysql_query("COMMIT", $id_conn);

$all_visit_curr_ip = mysql_query("SELECT COUNT(*) FROM visit WHERE addr_ip='".$_SERVER['REMOTE_ADDR']."'",$id_c);
$row_all_visit_curr_ip = mysql_fetch_array($all_visit_curr_ip);

$all_visit_curr_day = mysql_query("SELECT COUNT(*) FROM visit WHERE day=DAY(CURRENT_DATE())",$id_c);
$row_all_visit_curr_day = mysql_fetch_array($all_visit_curr_day);

$all_visit_curr_month = mysql_query("SELECT DAY , COUNT( * ) FROM visit WHERE MONTH = MONTH( CURRENT_DATE( ) ) GROUP BY DAY",$id_c);
$cnt = mysql_num_rows($all_visit_curr_month) - 1;

$all_visit_in_month = mysql_query("SELECT COUNT(*) FROM visit",$id_c);
$row_all_visit_in_month = mysql_fetch_array($all_visit_in_month);

$chart = "http://chart.apis.google.com/chart?";
$cht="ls";
$chs="640x320";
$chds="0,100";

switch($m = date("n")) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		$m = 31;
		break;
	case 2:
		$y = date("L");
		if(!$y)
			$m = 28;
		else
			$m = 29;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		$m = 30;
		break;
	default:
		die("Problem z wyliczeniem liczby dni w miesiącu");
}

$i = 0;
while($i < $m) {
	$tab[$i] = 0;
	$i++;
}

while($row_all_visit_curr_month = mysql_fetch_row($all_visit_curr_month)) {
		$tab[$row_all_visit_curr_month[0] - 1] = $row_all_visit_curr_month[1];
}

$i = 0;
while($i < $m) {
	if($i == ($m - 1))
		$chd .= $tab[$i];
	else 
		$chd .= $tab[$i].",";

	if($i == ($m - 1))
		$chl .= ($i + 1);
	else
		$chl .= ($i + 1)."|";

	$i++;
}

$url = $chart."cht=".$cht."&chd=t:".$chd."&chs=".$chs."&chl=".$chl."&chds=".$chds."&chxt=x,y,r,t";

mysql_free_result($all_visit_curr_month);
mysql_free_result($all_visit_curr_day);
mysql_free_result($all_visit_curr_ip);
mysql_close($id_c);

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd"> 

<html>
	<head>
		<meta content="text/html; charset=utf-8" http-equiv="Content-Type">
	</head>
	<body>
		<center>Jesteś tutaj <b><?php echo $row_all_visit_curr_ip[0]; ?></b> raz.</center>
		<center>W sumie <b><?php echo $row_all_visit_curr_day[0]; ?></b> przeglądarką dzisiaj, czyli <b><?php echo date("d-m-Y"); ?></b>.</center>
		<center>Statystyki w miesiącu - ogólem <b><?php echo $row_all_visit_in_month[0]; ?></b> wizyt(y).</center>
		<center><img src="<?php echo $url; ?>"/></center>
	</body>
</html>
<?php
}
?>
