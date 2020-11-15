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

$objDBFile = new DB_FILE("test.csv", ";");

$insert[0] = "1";
$insert[1] = "name";
$insert[2] = "surname";
$insert[3] = "city";
$insert[4] = "organization";
$objDBFile->insert($insert);
$insert[0] = "2";
$insert[1] = "k";
$insert[2] = "o";
$insert[3] = "r";
$insert[4] = "h";
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

class cenowki {
	private $dpi;
	private $x_mm;
	private $y_mm;
	
	private $x;
	private $y;
	
	private $image_resource;
	
	private $bg_r, $bg_g, $bg_b;
	
	private $background_color;
	
	private $colors;
	
	private $cell_width;
	private $cell_height;
	
	private $lenght_cell_width;
	private $lenght_cell_height;
	private $lenght_matrix;
	private $idx_matrix;
	
	private $bounding_left;
	private $bounding_right;
	private $bounding_up;
	private $bounding_down;
	
	private $current_x;
	private $current_y;
	private $ceil_x;
	private $ceil_y;
	
	private $bw_font;

	public function __construct() {
		$this->dpi = 0;
		$this->x_mm = 0;
		$this->y_mm = 0;
		$this->x = 0;
		$this->y = 0;
		$this->image_resource = NULL;
		$this->bg_r = 0;
		$this->bg_g = 0;
		$this->bg_b = 0;
		$this->background_color = 0;
		$this->colors = array();
	
		$this->current_x = 0.00;
		$this->current_y = 0.00;
		$this->ceil_x = 0;
		$this->ceil_y = 0;
		
		$this->cell_width = 0.00;
		$this->cell_height = 0.00;
	
		$this->lenght_cell_width = 0;
		$this->lenght_cell_height = 0;
		$this->lenght_matrix = 0;
		$this->idx_matrix = 0;
	
		$this->bounding_left = 5;
		$this->bounding_right = 5;
		$this->bounding_up = 5;
		$this->bounding_down = 5;
		
		$this->bw_font = 1;
		//from php.net
		putenv('GDFONTPATH=' . realpath('.'));
	}

	public function __destruct() {

	}

	// px
	private function set_cell_width() {
		$this->cell_width = $this->x / $this->lenght_cell_width;
	}
	
	// px
	private function set_cell_height() {
		$this->cell_height = $this->y / $this->lenght_cell_height;
	}
	
	// px
	private function get_cell_width() {
		return (float)$this->cell_width;
	}
	
	// px
	private function get_cell_height() {
		return (float)$this->cell_height;
	}
	
	private function inc_current_x() {
		$this->current_x += $this->get_cell_width();
	}
	
	// px
	private function inc_current_y() {
		$this->current_y += $this->get_cell_height();
	}
	
	// px
	private function get_current_x() {
		return $this->current_x;
	}
	
	// px
	private function get_current_y() {
		return $this->current_y;
	}
	
	// int
	private function lenght_matrix() {
		$this->lenght_matrix = $this->lenght_cell_width * $this->lenght_cell_height;
	}
	
	// int
	private function inc_idx_matrix() {
		$this->idx_matrix++;
	}
	
	// int
	private function get_idx_matrix() {
		return $this->idx_matrix;
	}
	
	
	private function t_widthlen($font_size,$font_name,$text) {
		$text = iconv('windows-1250','utf-8',$text);
		list($llx,$lly,$lrx,$lry,$urx,$ury,$ulx,$uly) = imagettfbbox($font_size,0,$font_name,$text);
		return ($urx - $ulx); // Prawy górny X - Lewy Górny X
	}

	private function t_heightlen($font_size,$font_name,$text) {
		$text = iconv('windows-1250','utf-8',$text);
		list($llx,$lly,$lrx,$lry,$urx,$ury,$ulx,$uly) = imagettfbbox($font_size,0,$font_name,$text);
		return ($lly - $uly); // Lewy Dolny X - Lewy Górny X
	}

	public function createimage($arguments) {	
		$this->x = ((($arguments['x_mm'] / 25.4) * $arguments['dpi']));
		$this->y = ((($arguments['y_mm'] / 25.4) * $arguments['dpi']));
		$this->x_mm = $arguments['x_mm'];
		$this->y_mm = $arguments['y_mm'];
		$this->dpi	= $arguments['dpi'];
		$this->image_resource = imagecreate($this->x, $this->y);
	}
	
	public function background_color($arguments) {
		list($r, $g, $b) = explode(":", $arguments['background']);
		$this->background_color = imagecolorallocate($this->image_resource, $r, $g, $b);
	}
	
	public function colors($arguments) {
		foreach($arguments['colors'] as $key => $val) {
			list($r, $g, $b) = explode(":", $val);
			$this->colors[$key] = imagecolorallocate($this->image_resource, $r, $g, $b);
		}
	}
	
	public function drawbox($color) {
		imageline($this->image_resource,0,0,$this->x - 1,0,$this->colors[$color]);
		imageline($this->image_resource,0,0,0,$this->y - 1,$this->colors[$color]);
		imageline($this->image_resource,0,$this->y - 1,$this->x - 1,$this->y - 1,$this->colors[$color]);
		imageline($this->image_resource,$this->x - 1,0,$this->x - 1,$this->y - 1,$this->colors[$color]);
	}
	
	public function drawcell($rows, $cols, $color) {
		$this->lenght_cell_width = $cols;
		$this->lenght_cell_height = $rows;
		$this->set_cell_width();
		$this->set_cell_height(); 
		$this->lenght_matrix();
		for($i = 0; $i <= $this->x; $i += $this->get_cell_width())
			imageline($this->image_resource,$i,0,$i,$this->y,$this->colors[$color]);
		for($i = 0; $i <= $this->y; $i += $this->get_cell_height())
			imageline($this->image_resource,0,$i,$this->x,$i,$this->colors[$color]);
	}
	
	public function next_cell() {
		if($this->get_current_x() + $this->get_cell_width() < $this->x) {
			$this->inc_current_x();
		} else if($this->get_current_y() < $this->y) {
			$this->current_x = 0;
			$this->inc_current_y();
		} else {
		}
	}

	private function drawtext($text,$align,$y,$font_size,$font_name,$color) {
		$text = iconv('windows-1250','utf-8',$text);
		switch($align) {
			case 'LEFT':
				$text_pos = $this->bounding_left;
			break;
			case 'CENTER':
				$text_pos = ($this->get_cell_width() - $this->t_widthlen($font_size,$font_name,$text))/2;
			break;
			case 'RIGHT':
				$text_pos = $this->get_cell_width() - $this->t_widthlen($font_size,$font_name,$text) - $this->bounding_right;
			break;
		}
		
		imagettftext($this->image_resource,$font_size,0,$this->get_current_x() + $text_pos,$this->get_current_y() + $y + $this->bounding_up,$this->colors[$color],$font_name,$text);
		
	}

	public function centertext($y,$nr,$font_size,$font_name,$color,$text) {
		$pos_y = $font_size * $nr + ($font_size/2);
		$this->drawtext($text,'CENTER',$y + $pos_y,$font_size,$font_name,$color);
	}
	
	public function lefttext($y,$nr,$font_size,$font_name,$color,$text) {
		$pos_y = $font_size * $nr + ($font_size/2);
		$this->drawtext($text,'LEFT',$y + $pos_y,$font_size,$font_name,$color);
	}
	
	public function righttext($y,$nr,$font_size,$font_name,$color,$text) {
		$pos_y = $font_size * $nr + ($font_size/2);
		$this->drawtext($text,'RIGHT',$y + $pos_y,$font_size,$font_name,$color);
	}
	
	public function centertext_multiline($text,$y,$font_size,$font_name,$color) {
		$pos_y = -$font_size;
		foreach($text as $t) {
			$t = iconv('windows-1250','utf-8',$t);
			$pos_y += $this->t_heightlen($font_size,$font_name,$t);
		}
		$tmp = ($this->get_cell_height()-$pos_y)/2;
		$pos_y = $tmp;		
		foreach($text as $t) {
			$t = iconv('windows-1250','utf-8',$t);
			$this->drawtext($t,'CENTER',$y + $pos_y,$font_size,$font_name,$color);
			$pos_y += $this->t_heightlen($font_size,$font_name,$t);
		}
	}

	public function output() {
		header('Content-type: image/png');
		return imagepng($this->image_resource);
	}
	
	public function output_file($file) {
		header('Content-type: image/file');
		imagepng($this->image_resource,$file);
	}

	public function show() {
		print_r($this->colors)."<br>\n";
		echo($this->background_color)."<br>\n";
		echo($this->image_resource)."<br>";
		echo($this->dpi)."<br>\n";
		echo($this->x)."<br>\n";
		echo($this->y)."<br>\n";
		echo($this->x_mm)."<br>\n";
		echo($this->y_mm)."<br>\n";
	}
	
	public function show_iterate() {
		echo $this->lenght_cell_width." * ".$this->lenght_cell_height." = ".$this->lenght_matrix."\n";
		echo "Cell width: ".$this->get_cell_width()."\n";
		echo "Cell height: ".$this->get_cell_height()."\n";
		echo "X px (x mm) :".$this->x." px (".$this->x_mm." mm)\n";
		echo "Y px (y mm) :".$this->y." px (".$this->y_mm." mm)\n";
		echo "\n";
		for($i = 0; $this->get_idx_matrix() < $this->lenght_matrix; $i++ ) {
			echo $this->get_idx_matrix().":\n";
			echo "\tCurrent X: ".$this->current_x."\n";
			echo "\tCurrent Y: ".$this->current_y."\n";
				if($this->get_current_x() < $this->x)
					$this->inc_current_x();
				else {
					$this->inc_current_y();
					$this->current_x = 0;
					if($this->get_current_y() > $this->y)
						return TRUE;
				}
				$this->inc_idx_matrix();
		}
	}
};

$arg = array(
	'x_mm' => 210,
	'y_mm' => 297,
	'dpi' => 72,
	'background' => '255:255:255',
	'colors' => array('red' => '255:0:0','green' => '0:255:0','blue' => '0:0:255')
);
			
$test = new cenowki();
$test->createimage($arg);
$test->background_color($arg);
$test->colors($arg);
$test->drawcell(8,4,'red');
$test->drawbox('red');


$test->centertext_multiline(array(0=>'A'),1,16,'FreeSerif.ttf','red');
$test->next_cell();

$test->centertext_multiline(array(0=>'B'),1,16,'FreeSerif.ttf','red');
$test->next_cell();

$test->centertext_multiline(array(0=>'C'),1,16,'FreeSerif.ttf','red');
$test->next_cell();

$test->centertext_multiline(array(0=>'D'),1,16,'FreeSerif.ttf','red');
$test->next_cell();

$test->centertext_multiline(array(0=>'E'),1,16,'FreeSerif.ttf','red');
$test->next_cell();

$test->centertext_multiline(array(0=>'F'),1,16,'FreeSerif.ttf','red');
$test->next_cell();

$test->centertext_multiline(array(0=>'G'),1,16,'FreeSerif.ttf','red');
$test->next_cell();

$test->centertext_multiline(array(0=>'Pierwsza...',1=>'...druga...',2=>'...trzecia...'),1,16,'FreeSerif.ttf','green');
$test->next_cell();

$test->centertext_multiline(array(0=>'Pierwsza...',1=>'...druga...',2=>'...trzecia...'),1,16,'FreeSerif.ttf','blue');
$test->next_cell();

$test->centertext(1,0,16,'FreeSerif.ttf','red','');
$test->centertext(1,1,16,'FreeSerif.ttf','red','CENTER');
$test->lefttext(1,2,16,'FreeSerif.ttf','red','LEFT');
$test->righttext(1,3,16,'FreeSerif.ttf','red','RIGHT');
$test->next_cell();

$i = 1;
foreach($objDBFile->select(1) as $text) {
	$test->centertext(1,$i,16,'FreeSerif.ttf','blue',$text);
	$i++;
}
$test->next_cell();

$test->centertext_multiline($objDBFile->select(2),1,16,'FreeSerif.ttf','blue');
$test->next_cell();

$i = 1;
foreach($objDBFile->select(3) as $text) {
	$test->centertext(1,$i,16,'FreeSerif.ttf','red',$text);
	$i++;
}
$test->next_cell();

$i = 1;
foreach($objDBFile->select(4) as $text) {
	$test->centertext(1,$i,16,'FreeSerif.ttf','green',$text);
	$i++;
}
$test->next_cell();

$test->output_file('1.png');

?>
