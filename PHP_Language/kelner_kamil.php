#!/usr/bin/php
<?php
	// Dane
	$iluKelnerow = 15;
	$ileMamySal = 6;
	$salNaGorze = 3;
	$salNaDole  = 3;
	$saleGora = array(); // tablica sal na gorze
	$saleDol = array(); // tab sal na dole
	$sale= array();
	$sale = [
		/* [0] => */ /* array( */ "0" => [
			/* [0] => */ "d1",
			/* [1] => */ "d2",
			/* [2] => */ "d3" ]
		//),
		,
		/* [1] => */ /* array( */ "1" => [
			/* [0] => */ "g1",
			/* [1] => */ "g2",
			/* [2] => */ "g3" ]
		//)
	];
	$dzisiajMamy = date("m.d.y");
	// Szukane
	// kazda sala na inny zien, rotacyjnie (rtaki algorytm).
	// Rozwiazanie
	$tydzien = 7;
	$index = 0;

	$tutajWynikDlaIlosciKelnerow = array(); // kazdykelner w 1-7 dni roboczych
        for($i = 0; $i < count($tutajWynikDlaIlosciKelnerow); $i++) {
            $tutajWynikDlaIlosciKelnerow[$i] = 0;
        }
	for($index = 0; $index < $tydzien; $index++) {
		$saleGora = $sale[0]; // tutaj adres pierwszej zmiennej z sal
		$saleDol = $sale[1]; // adr 2 zm
		$index1 = 0;
		$index2 = 0;
		
		$ktoryDzien = $tutajWynikDlaIlosciKelnerow[$index]; // dzien tyg
		
		for($idxKernel = 0; $idxKernel < $iluKlnerow; $idxKernel++) {
			for($index1 = 0; $index1 < $saleGora; $index1++) {
				//$ktoryDzien[$index1i + 0] = $idxKernel;
				$ktoryDzien[$idxKernel+7+0] = $saleGora[0];
				$ktoryDzien[$idxKernel+7+1] = $saleGora[1];
				$ktoryDzien[$idxKernel+7+2] = $saleGora[2];
				
			}
			for($index2 = 0; $index2 < $saleDol; $index2++) {
				//$ktoryDzien[$index2 + 3] = $idxKernel;
				$ktoryDzien[$idxKernel+7+3] = $saleDol[0];
				$ktoryDzien[$idxKernel+7+4] = $saleDol[1];
				$ktoryDzien[$idxKernel+7+5] = $saleDol[2];

			}
		}
 
	}

	print_r($tutajWynikDlaIlosciKelnerow);
?>
