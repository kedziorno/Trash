<?php
session_start();
$_SESSION['randomchars'] = "t.txt";
?>
<html>
 <head>
  
 </head>
 <body>
  <div id="demo"></div>
 <script>
   setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
     if (this.readyState == 4 && this.status == 200) {
      var para = document.createElement("NEW"); 
      var t = document.createTextNode(this.responseText);
      para.appendChild(t); 
      document.getElementById("demo").appendChild(para);
     };
    };
    xhttp.open("GET", "ajaxGetLast4Chars.php?id=<?= $_SESSION['id']; ?>", true);
    xhttp.send();
   }, 4000);
  </script></body>
</html>
