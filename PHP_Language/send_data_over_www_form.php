<?php

interface DB {
	public function insert($tab);
	public function update($tab);
	public function select($index);
	public function delete($index);
};

class DB_FILE implements DB {
	private $handle;
	private $delimiter;
	private $filename;

	public function __construct($file, $delimiter) {
		if(file_exists($file))
			unlink($file);
		touch($file);	
		$this->delimiter = $delimiter;
		$this->filename = $file;
	}

	public function __destruct() {
		$this->drop();
	}
	
	public function insert($tab) {
		$this->open();
		$str = implode($this->delimiter, $tab);
		fputs($this->handle, $str."\n");
		$this->close();
	}

	public function update($tab) {
		$this->open();
		$i1 = 0; $i2 = 0;
		while($buffer = fgets($this->handle)) {
			$buffer = substr($buffer, 0, strlen($buffer) - 1);
			$str[$i1] = explode($this->delimiter, $buffer);
			$i1++;
		}

		for($i2 = 0 ; $i2 < $i1; $i2++) {
			$tmp = $str[$i2];
			$id = $tmp[0];
			$index = $tab[0];
			if($id == $index) {
				$str[$i2] = $tab;
			}
		}

		file_put_contents($this->filename, '');
		for($i2 = 0; $i2 < $i1; $i2++) {
			$s = implode($this->delimiter, $str[$i2]);
			fwrite($this->handle, $s."\n");
		}
		$this->close();
	}

	public function select($index) {
		$this->open();
		while($buffer = fgets($this->handle)) {
			$tmp = explode($this->delimiter, $buffer);
			$id = $tmp[0];
			if($id == $index)
				return $tmp;
		}
		$this->close();
	}

	public function delete($index) {
		$this->open();
		$i1 = 0; $i2 = 0;
		while($buffer = fgets($this->handle)) {
			$buffer = substr($buffer, 0, strlen($buffer) - 1);
			$str[$i1] = explode($this->delimiter, $buffer);
			$i1++;
		}

		for($i2 = 0 ; $i2 < $i1; $i2++) {
			$tmp = $str[$i2];
			$id = $tmp[0];
			if($id == $index) {
				unset($str[$i2]); //$str[$i2] = $tab;
			}
		}

		file_put_contents($this->filename, '');
		for($i2 = 0; $i2 < $i1; $i2++) {
			if(!isset($str[$i2]))
				continue;
			$s = implode($this->delimiter, $str[$i2]);
			fwrite($this->handle, $s."\n");
		}
		$this->close();
	}

	public function content() {
		$this->open();
		while($buffer = fgets($this->handle)) {
			print $buffer;
		}
		$this->close();
	}

	private function drop() {
		$this->close();
		unlink($this->filename);	
	}

	private function open() {
		$this->handle = fopen($this->filename, "a+");
	}

	private function close() {
		if(is_resource($this->handle))
			fclose($this->handle);
	}
};

/*
$objDBFile = new DB_FILE("test.csv", ";");

$insert[0] = "1";
$insert[1] = "name";
$insert[2] = "surname";
$insert[3] = "city";
$insert[4] = "organization";
$objDBFile->insert($insert);
$insert[0] = "2";
$insert[1] = "konrad";
$insert[2] = "oblak";
$insert[3] = "rabka";
$insert[4] = "home";
$objDBFile->insert($insert);
$insert[0] = "3";
$insert[1] = "test";
$insert[2] = "testowy";
$insert[3] = "testowo";
$insert[4] = "testowa";
$objDBFile->insert($insert);
$insert[0] = "4";
$insert[1] = "aaaa";
$insert[2] = "bbbb";
$insert[3] = "cccc";
$insert[4] = "dddd";
$objDBFile->insert($insert);

$objDBFile->content();

print_r($objDBFile->select(1));
print_r($objDBFile->select(2));
print_r($objDBFile->select(3));
print_r($objDBFile->select(4));

$update[0] = "2";
$update[1] = "KONRAD";
$update[2] = "OBŁĄK";
$update[3] = "RABKA";
$update[4] = "HOME";
$objDBFile->update($update);

$update[0] = "1";
$update[1] = "NAME1";
$update[2] = "SURENAME2";
$update[3] = "CITY3";
$update[4] = "ORGANIZATION4";
$objDBFile->update($update);

print_r($objDBFile->select(1));
print_r($objDBFile->select(2));
print_r($objDBFile->select(3));
print_r($objDBFile->select(4));

$objDBFile->content();

$objDBFile->delete(1);
print_r($objDBFile->select(2));
$objDBFile->delete(3);
print_r($objDBFile->select(4));

$objDBFile->content();

$objDBFile->delete(1);
$objDBFile->delete(2);
$objDBFile->delete(3);
$objDBFile->delete(4);

$objDBFile->content();
*/

$objDB = new DB_FILE("db.csv", ";");

$i[0] = "1";
$i[1] = "kjoblak85@gmail.com";
$i[2] = "0123456789ABCDEF";

$objDB->insert($i);

class Token {
	private $cookie_key = '';
	private $id = 0;
	private $email = '';

	public function __construct($objdbfile, $index) {
		$row = $objdbfile->select($index);
		$this->id = $row[0];
		$this->email = $row[1];
		$this->cookie_key = $row[2];
	}
	
	public function getCookieKey() {
		return $this->cookie_key;
	}

	public function getId() {
		return $this->id;
	}

	public function getEmail() {
		return $this->email;
	}

	public function getToken() {
		$token  = $this->getCookieKey();
		$token .= $this->getId();
		$token .= $this->getEmail();
		return md5($token);
	}	
};

$objToken = new Token($objDB, 1);

print $objToken->getCookieKey()."\n";
print $objToken->getId()."\n";
print $objToken->getEmail()."\n";
print $objToken->getToken()."\n";

class MyCurl {
	private $url = '';
	private $opt = array();
	private $post_opt = array();
	private $curl = null;
	private $host = '';
	private $port = '';

	public function __construct($host,$port) {
		if(isset($host) && isset($port)) {
			$this->host = $host;
			$this->port = $port;
			$this->curl = curl_init();
		} else {
			die("(".$host.":".$port.")\n");
		}
	}
	
	public function add_opt($name,$val) {
		if(isset($name) && isset($val)) {
			if(isset($this->opt[$name]))
				unset($this->opt[$name]);
			if($name == CURLOPT_URL) {
				$val = "http://".$this->host.":".$this->port.$val;
			}
			$this->opt[$name] = $val;
		}
	}
	
	public function curl_run($bool_file) {
		if($bool_file == 1 && $this->opt['CURLOPT_RETURNTRANSFER'] == 1) {
			curl_setopt_array($this->curl,$this->opt);
			if(!file_exists("log_curl.".$date("%Y-%m-%d"))) {
				unlink("log_curl.".$date("%Y-%m-%d"));
			}
			$f = fopen("log_curl.".$date("%Y-%m-%d"),"wt");
			fwrite($f,"******* *****\n".curl_exec($this->curl)."\n");
			fclose($f);
		} else if($bool_file == 0) {
			
			foreach($this->opt as $key=>$val) {
				curl_setopt($this->curl,$key,$val);
				if($key == CURLOPT_POSTFIELDS) {
					print_r($val);
				}
			}
			curl_exec($this->curl);
			curl_close($this->curl);
			$this->curl = curl_init();
			unset($this->opt);
			$this->opt = array();
		}
	}
};

class boundary {
	private $boundary_head = '';
	private $boundary_body = '';
	private $boundary_end = '';

	public function __construct($len) {
		if(is_numeric($len) && $len > 0) {
			$minus_head = 55-$len;
			$minus_body = 57-$len;
			$minus_end = 59-$len;
			if($minus_head < 0 OR $minus_body < 0 || $minus_end < 0) {
				die("boundary : liczba znakow uzupelniajacych mniejsza od 0 - head($minus_head), body($minus_body), end($minus_end)\n");
			}
			$rnd = $this->random_bound($len);
			$this->boundary_head = str_repeat("-",$minus_head).$rnd;
			$this->boundary_body = str_repeat("-",$minus_body).$rnd;
			$this->boundary_end = str_repeat("-",$minus_end-2).$rnd.str_repeat("-",2);
		}
	}
	
	public function get_bounds() {
		return array(0 => $this->boundary_head, 1 => $this->boundary_body, 2 => $this->boundary_end);
	}
	
	private function random_bound($len) {
		$ret = '';
		if(is_numeric($len) && $len>0) {
			for($len_idx = 0; $len_idx < $len; $len_idx++) {
				$min = mt_rand(0,9);
				$max = mt_rand(0,9);
				if($min < $max) {
					$ret .= mt_rand($min,$max);
				} else 
				if($min > $max) {
					$ret .= mt_rand($max,$min);
				} else 
					$ret .= $min;
			}
			return $ret;
		}
	}
};

class headers_http extends boundary {
	
	private $headers_type = '';
	private $boundary_len = 0;
	private $boundary_tab = array();
	private $headers_name = array();
	private $headers_multi = array();
	private $headers_multi_name = '';
	private $file_content_type = 0;

	public function __construct($args) {
		if(is_array($args)) {
			if(isset($args['type']) && is_string($args['type'])) {
				$this->headers_type = $args['type'];
			} else {
				$this->headers_type = 'multipart/form-data';
			}
			if(isset($args['boundary_len']) && is_numeric($args['boundary_len'])) {
				$this->boundary_len = $args['boundary_len'];
			} else {
				$this->boundary_len = 28;
			}
			if(isset($args['file_content_type']) && is_numeric($args['file_content_type'])) {
				$this->file_content_type = $args['file_content_type'];
			} else {
				$this->file_content_type = 0;
			}			
			parent::__construct($this->boundary_len);
			$this->boundary_tab = parent::get_bounds();
			
		}
	}
	
	public function add_area($name,$value) {
		if(isset($name) && isset($value)) {
			$this->headers_name[$name] = $value;
		}
	}
	
	public function multi_area($name,$value) {
		if(isset($name) && isset($value)) {
			array_push($this->headers_multi, $name."=".$value);
		}
	} 
	
	public function del_area($name) {
		if(isset($this->headers_name[$name])) {
			unset($this->headers_name[$name]);
		}
	}
	
	public function get_curl($urlencoded) {
		if($this->headers_type == 'multipart/form-data') {
			$dont_ex = 0;
			$h_body = "\r\n";
			foreach($this->headers_name as $key => $val) {
				$val = trim($val);
				if($val[0] == '@') {
					$tmp = explode("/",$val);
					$filename = $tmp[count($tmp)-1];
					if(!file_exists(substr($val,2,strlen($val)))) {
						$dont_ex = 1;
					}
					if($dont_ex == 0) {
						list($file,$ext) = explode(".",$filename);
						switch($ext) {
							case "jpg":
							case "jpeg":
								$ftype = "image/jpeg";
							break;
							default:
								echo("Nieznane rozszezenie wysylanego pliku (".$ext.")\n");
						}
					}
					if($dont_ex == 0) {
						if($this->file_content_type == 0) {
							// ver 0						
							$ctx = file_get_contents(substr($val,2,strlen($val)));
							$ctx = strtr($ctx,"\x00","_");
						}
						if($this->file_content_type == 1) {
							// ver 3
							$ctx = '';
							$dataString = file_get_contents(substr($val,2,strlen($val)));
							$arrData = unpack("H*", $dataString);
							for($i=1;$i<(strlen($arrData[1])/2);$i +=2) {
								$ctx .= "\x".substr($arrData[1],$i,2);
							}
						}
					}
					$h_body .= $this->boundary_tab[1]."\r\n";
					if($dont_ex == 1) 
						$h_body .= "Content-Disposition: form-data; name=\"".$key."\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n";
					else if($dont_ex == 0)
						$h_body .= "Content-Disposition: form-data; name=\"".$key."\"; filename=\"".$filename."\"\r\nContent-Type: ".$ftype."\r\n\r\n".$ctx."\r\n";
				} else {
					$h_body .= $this->boundary_tab[1]."\r\n";
					$h_body .= "Content-Disposition: form-data; name=\"".$key."\"\r\n\r\n$val\r\n";
				}
			}
			if(count($this->headers_multi) > 0) {
				for($i=0;$i<count($this->headers_multi);$i++) {
					$tab = $this->headers_multi[$i];
					list($key, $val) = explode("=", $tab);
					$h_body .= $this->boundary_tab[1]."\r\n";
					$h_body .= "Content-Disposition: form-data; name=\"".$key."\"\r\n\r\n$val\r\n";
				}
			}
			$h_body .= $this->boundary_tab[2];
			
			$h_len = 'Content-Length: '.strlen($h_body)."";
			$h_type = 'Content-Type: '.$this->headers_type.'; boundary='.$this->boundary_tab[0];
			return array(0=>$h_type,1=>$h_len,2=>$h_body);
		}
		if($this->headers_type == 'application/x-www-form-urlencoded') {
			if($urlencoded == 'urlencode') {
				$ret = '&';
				foreach($this->headers_name as $key => $val) {
					if($val[0] == '@') {
						$ret .= urlencode($key)."=&";
					} else {
						$ret .= urlencode($key)."=".urlencode($val)."&";
					}
				}
				return substr($ret,1,strlen($ret)-2);
			} else if($urlencoded == 'none') {
				$ret = '&';
				foreach($this->headers_name as $key => $val) {
					if($val[0] == '@') {
						$ret .= ($key)."=&";
					} else {
						$ret .= ($key)."=".($val)."&";
					}
				}
				return substr($ret,1,strlen($ret)-2);
			}
		}
	}
	
	public function get_str_headers() {
		$tmp = $this->get_curl('XXX');
		return $tmp[0]."\r\n".$tmp[1]."\r\n".$tmp[2];
	}
};

$mycurl = new mycurl("kndno.vot.pl","80");

$args['type'] = "multipart/form-data";
$args['boundary_len'] = 28;
$args['file_content_type'] = 0;

$date = date("Y-m-d H:i");
$name = "Konrad";
$gt = "Hello";

$h = new headers_http($args);
$h->add_area("Time", $date);
$h->add_area("Name",$name);
$h->add_area("Greeting", $gt);
$h->add_area("Token", $objToken->getToken());
$h->add_area("ImageJpeg","@/1.jpg");
$h->add_area("SUBMIT","SEND");

$mycurl->add_opt(CURLOPT_URL, '/o/index.php');
$mycurl->add_opt(CURLOPT_HEADER, 0);
$mycurl->add_opt(CURLOPT_NOBODY, 0);
$mycurl->add_opt(CURLOPT_USERAGENT, 'CURL');
$mycurl->add_opt(CURLOPT_TIMEOUT, 30);
$mycurl->add_opt(CURLOPT_RETURNTRANSFER, 1);
$mycurl->add_opt(CURLOPT_POST, 1);
$mycurl->add_opt(CURLOPT_HTTPHEADER, $h->get_curl('XXX'));
$mycurl->curl_run(0);

/*
//np 1
print "\nPrzyklad 1:\n";
$args['type'] = "application/x-www-form-urlencoded";
$args['boundary_len'] = 32;
$args['file_content_type'] = 0;
$h = new headers_http($args);
$h->add_area("NP1_1","test1");
$h->add_area("NP1_2","test2");
$h->add_area("NP1_3","test3");
$h->multi_area("MUL_1", "test1");
$h->multi_area("MUL_1", "test2");
$h->multi_area("MUL_1", "test3");
$h->multi_area("MUL_2", "test1");
$h->multi_area("MUL_2", "test2");
$h->add_area("f","1.jpg");
$h->add_area("SUBMIT","CONNECT");
print_r($h->get_curl('none'));
//print_r($h->get_curl('urlencode'));

//np 2
print "\nPrzyklad 2:\n";
$args['type'] = "multipart/form-data";
$args['boundary_len'] = 32;
$args['file_content_type'] = 0;
$h = new headers_http($args);
$h->add_area("NP2_1","test1");
$h->add_area("NP2_2","test2");
$h->add_area("NP2_3","test3");
$h->add_area("f","@/1.jpg");
$h->multi_area("MUL_1", "test1");
$h->multi_area("MUL_1", "test2");
$h->multi_area("MUL_1", "test3");
$h->multi_area("MUL_2", "test1");
$h->multi_area("MUL_2", "test2");
$h->add_area("SUBMIT","CONNECT");
print_r($h->get_curl('none'));
*/
?>
