<?php
//https://stackoverflow.com/questions/1159216/how-can-i-get-php-to-produce-a-backtrace-upon-errors
function process_error_backtrace($errno, $errstr, $errfile, $errline, $errcontext) {
    if(!(error_reporting() & $errno))
        return;
    switch($errno) {
    case E_WARNING      :
    case E_USER_WARNING :
    case E_STRICT       :
    case E_NOTICE       :
    case E_USER_NOTICE  :
        $type = 'warning';
        $fatal = false;
        break;
    default             :
        $type = 'fatal error';
        $fatal = true;
        break;
    }
    $trace = array_reverse(debug_backtrace());
    array_pop($trace);
    if(php_sapi_name() == 'cli') {
        echo 'Backtrace from ' . $type . ' \'' . $errstr . '\' at ' . $errfile . ' ' . $errline . ':' . "\n";
        foreach($trace as $item)
            echo '  ' . (isset($item['file']) ? $item['file'] : '<unknown file>') . ' ' . (isset($item['line']) ? $item['line'] : '<unknown line>') . ' calling ' . $item['function'] . '()' . "\n";
    } else {
        echo '<p class="error_backtrace">' . "\n";
        echo '  Backtrace from ' . $type . ' \'' . $errstr . '\' at ' . $errfile . ' ' . $errline . ':' . "\n";
        echo '  <ol>' . "\n";
        foreach($trace as $item)
            echo '    <li>' . (isset($item['file']) ? $item['file'] : '<unknown file>') . ' ' . (isset($item['line']) ? $item['line'] : '<unknown line>') . ' calling ' . $item['function'] . '()</li>' . "\n";
        echo '  </ol>' . "\n";
        echo '</p>' . "\n";
    }
    if(ini_get('log_errors')) {
        $items = array();
        foreach($trace as $item)
            $items[] = (isset($item['file']) ? $item['file'] : '<unknown file>') . ' ' . (isset($item['line']) ? $item['line'] : '<unknown line>') . ' calling ' . $item['function'] . '()';
        $message = 'Backtrace from ' . $type . ' \'' . $errstr . '\' at ' . $errfile . ' ' . $errline . ': ' . join(' | ', $items);
        error_log($message);
    }
    if($fatal)
        exit(1);
}

//set_error_handler('process_error_backtrace');
?>
<?php

$context = stream_context_create(
array(
		'http'=>array(
			'header'  => "Authorization: Basic " . base64_encode("$username:$password")
		),
		"ssl"=>array(
            "verify_peer"=>false,
            "verify_peer_name"=>false,
        ),
));

// https://i-mscp.net/index.php/Thread/4208-Problem-with-PHP-function-file-get-contents/
function checkWrappers() {
	$w = stream_get_wrappers();
	echo 'openssl: ';echo extension_loaded ('openssl') ? 'yes':'no'; echo "\n";
	echo 'http wrapper: '; echo in_array('http', $w) ? 'yes':'no'; echo "\n";
	echo 'https wrapper: '; echo in_array('https', $w) ? 'yes':'no'; echo "\n";
	echo 'wrappers: '; var_dump($w);
	die();
}

// http://stackoverflow.com/a/12446906
function getSslPage($url) {
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_setopt($ch, CURLOPT_HEADER, false);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_REFERER, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
	$result = curl_exec($ch);
	curl_close($ch);
	return $result;
}

$out = array();
function flatten($data, $parent = "root") {            
    global $out;
    
    $chain = "";
    
    foreach( $data as $k => $v ) {
        $chain = $parent . "." . $k;
        if( is_array($v) ) {
            $out[$chain] = flatten($v, $chain);
        } else {
            $out[$chain] = $v;
        }
    }
    
    //ksort($out);
    
    return $out;
}

function putArray($fp, $val) { 
        $firstLine = true; 
        foreach($val as $k1=>$v1) { 
                $header = ""; 
                $values = ""; 
                foreach($v1 as $k2 => $v2) { 
                        if($firstLine) { 
                                $header .= "$k2;"; 
                        } 
                        $values .= "$v2;"; 
                } 
                if($firstLine) { 
                        fputcsv($fp, explode(";",$header)); 
                        $firstLine = false; 
                } 
                fputcsv($fp, explode(";",$values)); 
                $header = ""; 
                $values = ""; 
        } 
} 

function putArrayRecurse($fp, $val) { 
        if(is_array($val))
            putArrayRecurse($fp, $val);
        else
        foreach($val as $k1=>$v1) { 
                $values = ""; 
                foreach($v1 as $k2 => $v2) {
                        $values .= "$v2;"; 
                } 
                fputcsv($fp, explode(";",$values)); 
                $values = ""; 
        } 
} 

function putArrayWithoutHeader($fp, $val) { 
        foreach($val as $k1=>$v1) { 
                $values = ""; 
                foreach($v1 as $k2 => $v2) { 
                        $values .= "$v2;"; 
                } 
                fputcsv($fp, explode(";",$values)); 
                $values = ""; 
        } 
} 

$a1 = "";
$out1 = array();
function rA($a, &$f) {
    global $a1;
    global $out1;
    foreach($a as $k=>$i) {
        if(is_array($k)) {
            $out1 = rA($k, $f);
        } else {
            $f .= $k."=".$i."\n";
        }
    }
    return $out1;
}

function putArrayAndKeysWithId($fp, $val, $id, $putKeys = true) {
    global $main_data_flat;
    global $a1;
    //$var11 = "";
    //$var22 = "";
    $firstLine = true; 
    foreach($val as $k1=>$v1) {
            $header = "ID;";
            $values = "$id;";
            foreach($v1 as $k2 => $v2) { 
                if($firstLine) { 
                    $header .= "$k2;"; 
                } 
                /* if($k2 === "stats") {
                    $var1 = flatten($v2);
                    rA($var1, $var11);
                    $var1 = "";
                    echo " ***** $var11\n";
                    $values .= $var11.";";
                } else if($k2 === "explain") {
                    $var2 = flatten($v2);
                    rA($var2, $var22);
                    $var2 = "";
                    echo "\n ***** $var22\n";
                    $values .= $var22.";";
                } else */
                    $values .= "$v2;";
        }
        //$var11 = "";
        //$var22 = "";
        if($firstLine) { 
            if($putKeys == true)
                fputcsv($fp, explode(";",$header)); 
            $firstLine = false; 
        } 
        fputcsv($fp, explode(";",$values)); 
        $header = ""; 
        $values = ""; 
    } 
}

function putArrayAndKeysWithIdAndGW($fp, $val, $id, $gw, $putKeys = true) {
    $firstLine = true; 
    foreach($val as $k1=>$v1) {
            $header = "ID;GW;";
            $values = "$id;$gw;";
            foreach($v1 as $k2 => $v2) { 
                if($firstLine) { 
                    $header .= "$k2;"; 
                } 
                $values .= "$v2;";
        }
        if($firstLine) { 
            if($putKeys == true)
                fputcsv($fp, explode(";",$header)); 
            $firstLine = false; 
        } 
        fputcsv($fp, explode(";",$values)); 
        $header = ""; 
        $values = ""; 
    } 
}

function putArrayLvl1($fp, $gk, $gv) {
	if(count($gv)) {
		foreach($gv as $k=>$v) {
			fwrite($fp, $gk.",".$k.",".$v."\n"); 
		}
	} else 
		fwrite($fp, "\n");
}

function putArrayLvl2($fp, $gk, $gv) { 
        foreach($gv as $gv1=>$gv2) { 
                foreach($gv2 as $gv21=>$gv22)  
                        fwrite($fp, $gk.",".$gv1.",".$gv21.",".$gv22."\n"); 
        } 
} 
 
function putArrayLvl3($fp, $gk, $gv) { 
        foreach($gv as $gv1=>$gv2) { 
                foreach($gv2 as $gv21=>$gv22) { 
                        foreach($gv22 as $gv221=>$gv222) { 
                                fwrite($fp, $gk.",".$gv1.",".$gv21.",".$gv221.",".$gv222."\n"); 
                        } 
                } 
        } 
}
?>
