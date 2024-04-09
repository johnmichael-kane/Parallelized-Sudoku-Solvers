import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Scanner;

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
		String filename = "../../data/" + args[0];
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
		ExactMatrix myMatrix = new ExactMatrix(vals);
		DancingLinkSolver solver = new DancingLinkSolver(myMatrix.finalMatrix, boardSize);
		Sudoku.endTime = System.currentTimeMillis(); // Record end time
		System.out.println("Solved Sudoku:");
		System.out.print("Time taken: " + (Sudoku.endTime - Sudoku.startTime) + " milliseconds\n");
		printSudokuBoard(boardSize, partitionSize, solver.getSolution());

		printSudokuBoardInFile(boardSize, partitionSize, solver.getSolution());


	}
}
