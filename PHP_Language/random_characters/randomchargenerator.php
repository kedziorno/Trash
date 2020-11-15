<?php
define(N, 3600);

$f = fopen("t.txt", "w");

for($i=N;$i>0;$i--) {
  $c = chr(rand(32, 126));
  fwrite($f,$c);
  sleep(1);
}
fclose($f);
?>
