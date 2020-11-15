<?php
session_start();

$fs = filesize($_SESSION['randomchars']);
$off = $fs-$_SESSION['id'];
//printf("%d - %d = %d", $fs, $_SESSION['id'], $off);

$f = fopen("t.txt", "r");
fseek($f,-$off,SEEK_END);
$chars = fread($f,$off);
fclose($f);

echo $chars;

$_SESSION['id'] = $fs;
?>
