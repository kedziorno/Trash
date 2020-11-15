/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication1;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

import java.math.*;
import java.util.Arrays;

/**
 *
 * @author ko
 */
public class goftest extends Application {
	boolean[][] PrevWorld1;
	boolean[][] PrevWorld2;
	boolean[][] CurrentWorld;
	boolean[][] NextWorld;
	int Dimension = 10;
	int Generation = 1009;

	boolean WorldIsTheSame(boolean[][] mA, boolean[][] mB) {
		for(int a = 0; a < mA.length; a++) {
			boolean[] tA = mA[a];
			boolean[] tB = mB[a];
			if(!Arrays.equals(tA, tB))
				return false;
		}
		return true;
	}

	boolean getCellAt(boolean[][] cw, int pX, int pY) {
		if(pX < 0)
			pX = 0;
		if(pY < 0)
			pY = 0;
		if(pX >= Dimension)
			pX = Dimension - 1;
		if(pY >= Dimension)
			pY = Dimension - 1;
		return cw[pX][pY];
	}

	int countNeighborhoods(boolean[][] cw, int pX, int pY) {
		int countAlive = 0;

		if(pY != 0) {
			if(getCellAt(cw, pX, pY - 1) == true)
				countAlive++;
		}
		if(pY != Dimension - 1) {
			if(getCellAt(cw, pX, pY + 1) == true)
				countAlive++;
		}
		if(pX != Dimension - 1) {
			if(getCellAt(cw, pX + 1, pY) == true)
				countAlive++;
		}
		if(pX != 0) {
			if(getCellAt(cw, pX - 1, pY) == true)
				countAlive++;
		}
		if(pX != 0 && pY != 0) {
			if(getCellAt(cw, pX - 1, pY - 1))
				countAlive++;
		}
		if(pX != Dimension - 1 && pY != 0) {
			if(getCellAt(cw, pX + 1, pY - 1) == true)
				countAlive++;
		}
		if(pX != 0 && pY != Dimension - 1) {
			if(getCellAt(cw, pX - 1, pY + 1) == true)
				countAlive++;
		}
		if(pX != Dimension - 1 && pY != Dimension - 1) {
			if(getCellAt(cw, pX + 1, pY + 1) == true)
				countAlive++;
		}

		return countAlive;
	}

	boolean LiveOrDead(boolean[][] cw, int pX, int pY) {
		int neighbor = this.countNeighborhoods(cw, pX, pY);
		if(getCellAt(cw, pX, pY) == true) {
			if((neighbor == 2) || (neighbor == 3))
				return true;
			else
				return false;
		} else {
			if(neighbor == 3)
				return true;
			else
				return false;
		}
	}

	@Override
	public void start(Stage primaryStage) {
		int countCells = 0;
		int RowColCells = 0;
		int OneDimension = 0;
		int halfGeneration0 = 0;

		//boolean[][] Picture[] =  {
//			{
//			{ true, false, true, false, true },
//			{ true, false, false, false, true },
//			{ true, false, false, false, true },
//			{ true, false, false, false, true },
//			{ true, false, true, false, true }
//			},
//			{
//			{ false, true, false },
//			{ true, true, true },
//			{ true, false, true },
//			{ false, true, false }
//			},
//			{
//			{ false, true, false },
//			{ false, false, true },
//			{ true, true, true }
//			},
//			{
//			{ true, true, true, true, true, true, true, true, true, true }
//			},
//			{
//			{ false, true, true, true, true },
//			{ true, false, false, false, true },
//			{ false, false, false, false, true },
//			{ true, false, false, true, false }
//			},
//			{
//			{ false, true, true, false, true, true, false },
//			{ false, true, true, false, true, true, false },
//			{ false, false, true, false, true, false, false },
//			{ true, false, true, false, true, false, true },
//			{ true, false, true, false, true, false, true },
//			{ true, true, false, false, false, true, true }
//			},
//			{
//			{ true, true, true }
//			}
		//};

		// Random picture
		boolean[][] RandomWorld = new boolean[Dimension][Dimension];
		for(int row = 0; row < Dimension; row++) {
			for(int col = 0; col < Dimension; col++) {
				RandomWorld[row][col] = Math.random() < 0.3;
			}
		}

		boolean[][] Picture[] = { RandomWorld };

		for(boolean[][] p : Picture) {
			int PictureColLen = p[0].length;
			int PictureRowLen = p.length;

			if(PictureColLen > PictureRowLen)
				RowColCells = PictureColLen * 3;
			else
			if(PictureRowLen > PictureColLen)
				RowColCells = PictureRowLen * 3;

			Dimension = (int)(PictureRowLen == PictureColLen ? PictureRowLen * 3 : RowColCells);

			boolean[][] CurrentWorld = new boolean[Dimension][Dimension];

			int startOfReplaceCells = 0;
			if(PictureRowLen < PictureColLen)
				startOfReplaceCells = halfGeneration0 - ((PictureColLen-1)*OneDimension)/2 + (OneDimension - (PictureRowLen/2));
			else
			if(PictureRowLen > PictureColLen)
				startOfReplaceCells = halfGeneration0 - ((PictureRowLen-1)*OneDimension)/2 + (OneDimension - (PictureColLen/2));

			for(int a = 0; a < PictureRowLen; a++) {
				for(int b = 0; b < PictureColLen; b++) {
					if(p[a][b] == true) {
						CurrentWorld[a+Dimension/3][b+Dimension/3] = true;
					} else {
						CurrentWorld[a+Dimension/3][b+Dimension/3] = false;
					}
				}
			}

			for(int row = 0; row < Dimension; row++) {
				for(int col = 0; col < Dimension; col++) {
					if(CurrentWorld[row][col] == true) {
						System.out.print('@');
					} else {
						System.out.print('.');
					}
					System.out.print(' ');
				}
				System.out.println("");
			}

			System.out.println("Generation: 0");

			PrevWorld1 = new boolean[Dimension][Dimension];
			PrevWorld2 = new boolean[Dimension][Dimension];

			int indexGeneration = 1;
			for(; indexGeneration < Generation; indexGeneration++) {
				if(WorldIsTheSame(PrevWorld2, CurrentWorld)) {
					System.out.println("Generation "+(indexGeneration-3)+" and "+(indexGeneration-1)+" is the same");
					break;
				}
				NextWorld = new boolean[Dimension][Dimension];
				for(int row = 0; row < Dimension; row++) {
					for(int col = 0; col < Dimension; col++) {
						NextWorld[row][col] = LiveOrDead(CurrentWorld, row, col);
					}
				}

				PrevWorld2 = PrevWorld1;
				PrevWorld1 = CurrentWorld;
				CurrentWorld = NextWorld;

				for(int row = 0; row < Dimension; row++) {
					for(int col = 0; col < Dimension; col++) {
						if(CurrentWorld[row][col] == true) {
							System.out.print('@');
						} else {
							System.out.print('.');
						}
						System.out.print(' ');
					}
					System.out.println("");
				}

				System.out.println("Generation: "+indexGeneration);

				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					// Do nothing
				}
			}
		}
		System.exit(0);
	}

	/**
	 * @param args the command line arguments
	 */
	public static void main(String[] args) {
		launch(args);
	}

}
