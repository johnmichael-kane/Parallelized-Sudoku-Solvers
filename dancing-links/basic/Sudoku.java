import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Scanner;

class ExactMatrix {

	public int[][] matrix;
	public Cell[][] finalMatrix;
	int sudokuSize;
	
	public ExactMatrix(int[][] matrix) {
		this.matrix = matrix;
		finalMatrix = new Cell[matrix.length*matrix.length*matrix.length][matrix[0].length*matrix[0].length*4];
		sudokuSize = this.matrix.length;
		initMatrix();
		makeFinalMatrix();
	}
	
	private void initMatrix() {
		for (int i=0;i<finalMatrix.length; i++) {
			for (int j=0;j<finalMatrix[0].length; j++){
				finalMatrix[i][j] = null;
			}
		}
		
	}
	//initialize a local Cell[][] at the start of here
	public void makeFinalMatrix() {
		for (int i=0;i<sudokuSize; i++) {
			for (int j=0;j<sudokuSize; j++){
				int currNum = matrix[i][j];
				fillInMatrix(i,j,currNum);
			}
		}
	}

	private void fillInMatrix(int row, int col, int number) {
		for (int i = 1; i <= sudokuSize; i++) {
				if (number!=0){
					fillCell(row, col, number);
				} else {
					for (int num=1; num<=sudokuSize; num++) {
						fillCell(row, col, num);
					}
				}
		}
	}
	//create a method to include the local Cell[][] finalMatrix as a
	private void fillCell(int row, int col, int number) {
		int matrixRow = row*matrix.length*matrix.length + col*matrix.length+number-1;
		int col1 = row*sudokuSize+col;
		int col2 = sudokuSize*sudokuSize+row*sudokuSize+number-1;
		int col3 = 2*sudokuSize*sudokuSize+col*sudokuSize+number-1;
		int col4 = 3*sudokuSize*sudokuSize+getGridNumber(row, col)*sudokuSize+number-1;
		finalMatrix[matrixRow][col1] = new Cell(row, col, number);
		finalMatrix[matrixRow][col2] = new Cell(row, col, number);
		finalMatrix[matrixRow][col3] = new Cell(row, col, number);
		finalMatrix[matrixRow][col4] = new Cell(row, col, number);
	}

	private int getGridNumber(int row, int col) {

		int gridSize = (int)Math.sqrt(sudokuSize);

		return (col/gridSize)+((int)(row/gridSize))*gridSize;
	}
}

public class Sudoku {

	private static int boardSize = 0;
	private static int partitionSize = 0;
	private static long startTime; // Start time for solving the puzzle
	private static long endTime; // End time for solving the puzzle


	// Method to format single-digit numbers with leading zero
	public static String n(int n) {
		return n > 9 ? "" + n : "0" + n;
	}

	public static void printSudokuBoard(int boardSize, int blocksize, int vals[][] ) {
		for (int y = 0; y < boardSize; y++) {
			if (y % blocksize == 0) {
				System.out.println(" ");
			}

			for (int x = 0; x < boardSize; x++) {
				if (x % blocksize == 0) {
					System.out.print("|");
				}
				System.out.print(vals[y][x] == 0 ? "  . " : " " + n(vals[y][x]) + " ");
			}
			System.out.println("|");
		}
		System.out.println("");
	}


	public static void printSudokuBoardInFile(int boardSize, int blocksize, int vals[][]) {
		try {
			FileWriter writer = new FileWriter(boardSize+"x"+boardSize+"_out.txt"); // Open file for writing
			writer.write("Time taken: " + (endTime - startTime) + " milliseconds\n"); // Write time taken to solve
			writer.write("Solved Sudoku:\n");
			for (int y = 0; y < boardSize; y++) {
				if (y % blocksize == 0) {
					writer.write("\n");
				}
				for (int x = 0; x < boardSize; x++) {
					if (x % blocksize == 0) {
						writer.write("|");
					}
					writer.write(vals[y][x] == 0 ? "  . " : " " + n(vals[y][x]) + " ");
				}
				writer.write("|\n");
			}
			writer.close(); // Close the writer after writing
		} catch (IOException e) {
			e.printStackTrace(); // Handle IO exception
		}
	}

	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		String filename = args[0];
		File inputFile = new File(filename);
		Scanner input = null;
		int[][] vals = null;
		ArrayList<Cell> valsToBeFilled = null;

		int temp = 0;
		int count = 0;

		try {
			input = new Scanner(inputFile);
			temp = input.nextInt();
			boardSize = temp;
			partitionSize = (int) Math.sqrt(boardSize);

			vals = new int[boardSize][boardSize];
			valsToBeFilled = new ArrayList<Cell>();

			int i = 0;
			int j = 0;
			while (input.hasNext()) {
				temp = input.nextInt();
				count++;

				vals[i][j] = temp;
				if (temp == 0) {
					valsToBeFilled.add(new Cell(i, j, 0));
				}
				j++;
				if (j == boardSize) {
					j = 0;
					i++;

				}
				if (j == boardSize) {
					break;
				}
			}
			input.close();
		} catch (FileNotFoundException exception) {
			System.out.println("Input file not found: " + filename);
		}
		if (count != boardSize * boardSize)
			throw new RuntimeException("Incorrect number of inputs.");

		System.out.println("\nSudoku Board:");
		printSudokuBoard(boardSize, partitionSize,vals);
		Sudoku.startTime = System.currentTimeMillis(); // Record start time


		//goes throught he
		ExactMatrix myMatrix = new ExactMatrix(vals);
		DancingLinkSolver solver = new DancingLinkSolver(myMatrix.finalMatrix, boardSize);




		Sudoku.endTime = System.currentTimeMillis(); // Record end time
		System.out.println("Solved Sudoku:");
		System.out.print("Time taken: " + (Sudoku.endTime - Sudoku.startTime) + " milliseconds\n");
		printSudokuBoard(boardSize, partitionSize,solver.solution);

		printSudokuBoardInFile(boardSize, partitionSize,solver.solution);


	}
}
