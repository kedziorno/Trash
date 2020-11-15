<?php
// this script get the data from fantasy premier league
// tested on php7, requires php-curl

$username = '';
$password = '';

$headers_login = array(
	'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0',
	'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
	'Accept-Language: en-US,en;q=0.5',
	'Accept-Encoding: gzip, deflate, br',
	'Connection: keep-alive',
	'Upgrade-Insecure-Requests: 1',
	'Pragma: no-cache',
	'Cache-Control: no-cache'
);

$postfields_login = array(
	"app" => "plfpl-web",
	"login" => $username,
	"password" => $password
);

$headers_content = array(
	'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:60.0) Gecko/20100101 Firefox/60.0',
	'Accept: application/json,text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
	'Accept-Language: en-US,en;q=0.5',
	'Accept-Encoding: gzip, deflate, br',
	'Connection: keep-alive',
	'Upgrade-Insecure-Requests: 1',
	'Pragma: no-cache',
	'Cache-Control: no-cache'
);

class ArrayValue implements JsonSerializable {
	public function __construct(array $array) { $this->array = $array; }
	public function jsonSerialize() { return $this->array; }
}

function loginToPage() {
	$avhl = new ArrayValue($GLOBALS['headers_login']);
	$curl_main_handler = curl_init();
	curl_setopt($curl_main_handler, CURLOPT_VERBOSE, 		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_SSL_VERIFYPEER,		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_HEADER, 		FALSE); // without headers return
	curl_setopt($curl_main_handler, CURLOPT_HTTPHEADER, 		$avhl->jsonSerialize());
	curl_setopt($curl_main_handler, CURLOPT_FOLLOWLOCATION, 	TRUE);
	curl_setopt($curl_main_handler, CURLOPT_AUTOREFERER, 		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_URL, 			'https://users.premierleague.com/accounts/login/');
	curl_setopt($curl_main_handler, CURLOPT_REFERER, 		'https://fantasy.premierleague.com/?state=success');
	curl_setopt($curl_main_handler, CURLOPT_RETURNTRANSFER, 	TRUE);

	curl_setopt($curl_main_handler, CURLOPT_COOKIESESSION, 		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_COOKIEJAR,		'cookie.txt');
	curl_setopt($curl_main_handler, CURLOPT_COOKIEFILE,		'cookie.txt');

	curl_setopt($curl_main_handler, CURLOPT_POST, 			1);
	curl_setopt($curl_main_handler, CURLOPT_POSTFIELDS, 		$GLOBALS['postfields_login']); 
	return curl_exec($curl_main_handler);
}

function getContent($url) {
	$avhc = new ArrayValue($GLOBALS['headers_content']);
	$curl_main_handler = curl_init();
	curl_setopt($curl_main_handler, CURLOPT_VERBOSE, 		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_SSL_VERIFYPEER,		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_HEADER, 		FALSE); // without headers return
	curl_setopt($curl_main_handler, CURLOPT_HTTPHEADER,		$avhc->jsonSerialize());
	curl_setopt($curl_main_handler, CURLOPT_FOLLOWLOCATION, 	TRUE);
	curl_setopt($curl_main_handler, CURLOPT_AUTOREFERER, 		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_URL, 			$url);
	curl_setopt($curl_main_handler, CURLOPT_RETURNTRANSFER, 	TRUE);

	curl_setopt($curl_main_handler, CURLOPT_COOKIESESSION, 		TRUE);
	curl_setopt($curl_main_handler, CURLOPT_COOKIEJAR,		'cookie.txt');
	curl_setopt($curl_main_handler, CURLOPT_COOKIEFILE,		'cookie.txt');

	return curl_exec($curl_main_handler);
}

print loginToPage();
print getContent('https://fantasy.premierleague.com/api/leagues-classic/0000/standings/?page_new_entries=1&page_standings=1');
?>
