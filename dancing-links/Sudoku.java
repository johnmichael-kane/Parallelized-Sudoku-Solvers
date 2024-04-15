import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

class ExactMatrix {

	public int[][] matrix;
	int sudokuSize;
	
	public ExactMatrix(int[][] matrix) {
		this.matrix = matrix;
		sudokuSize = this.matrix.length;
		
	}
	
	//rather than creating finalMatrix, initMatrix, then makeFinalMatrix, we keep it all for that function
	public Cell[][] makeFinalMatrix() {
		Cell[][] finalMatrix = new Cell[matrix.length*matrix.length*matrix.length][matrix[0].length*matrix[0].length*4];
		initMatrix(finalMatrix);
		for (int i=0; i<sudokuSize; i++) {
			for (int j=0; j<sudokuSize; j++){
				fillInMatrix(finalMatrix, i, j, matrix[i][j]);
			}
		}
		return finalMatrix;
	}

	private void initMatrix(Cell[][] finalMatrix) {
		for (int i=0;i<finalMatrix.length; i++) {
			for (int j=0;j<finalMatrix[0].length; j++){
				finalMatrix[i][j] = null;
			}
		}
		
	}

	private void fillInMatrix(Cell[][] finalMatrix, int row, int col, int number) {
		for (int num = 1; num <= sudokuSize; num++) {
			if (number == 0 || num == number) {
				fillCell(finalMatrix, row, col, num);
			}
		}
	}
	
	private void fillCell(Cell[][] finalMatrix, int row, int col, int number) {
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
	private static long startTime; 
	private static long endTime; 
	private static final AtomicReference<int[][]> solutionMatrix = new AtomicReference<>();


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

	//allows a copy without accessing the same information
	private static int[][] deepCopy(int[][] original) {
		if (original == null) {
			return null;
		}
		final int[][] result = new int[original.length][];
		for (int i = 0; i < original.length; i++) {
			result[i] = original[i].clone(); // This clones the inner array
		}
		return result;
	}

	private static void shutdownAndAwaitTermination(ExecutorService pool){
		pool.shutdown(); // Disable new tasks from being submitted
		try {
			// Wait a while for existing tasks to terminate
			if (!pool.awaitTermination(60, TimeUnit.SECONDS)) {
				pool.shutdownNow(); // Cancel currently executing tasks
				// Wait a while for tasks to respond to being cancelled
				if (!pool.awaitTermination(60, TimeUnit.SECONDS))
					System.err.println("Pool did not terminate");
			}
		} catch (InterruptedException ie) {
			// (Re-)Cancel if current thread also interrupted
			pool.shutdownNow();
			// Preserve interrupt status
			Thread.currentThread().interrupt();
		}
	}
	
	private static List<int[][]> generateStartingBoards(int[][] originalBoard, int blockSize, int numBoards) {
		List<int[][]> boards = new ArrayList<int[][]>(); // Specify the type inside <>
		if(numBoards == 1){
			boards.add(deepCopy(originalBoard));
		}
		else{
			for (int i = 0; i < numBoards; i++) {
				int[][] newBoard = deepCopy(originalBoard); 
				// ... apply BFS logic to fill first X empty spaces ...
				boards.add(newBoard);
			}
		}
		
		return boards;
	}

	private static int parseNumThreads(String input){
		try {
            int numThreads = Integer.parseInt(input);
            if (numThreads == 1 || numThreads == 2 || numThreads == 4 || numThreads == 8) {
                return numThreads;
            }
        } catch (NumberFormatException ex) {
            System.out.println("Number format exception for input string: " + input);
        }
        return -1; // Indicate invalid input
    }
	
	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		if(args.length < 2) { //error for less than 1 args
			System.out.println("Usage: java Sudoku <FILE_NAME> <NUM_THREADS>");
            return;
		}

		String filename = args[0];
		int numThreads = parseNumThreads(args[1]);
		if (numThreads == -1){
			System.out.println("Invalid number of threads. Please use 1, 2, 4, or 8.");
            return;
		}

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
		Sudoku.startTime = System.currentTimeMillis(); // Record start time; code starts here

		
		ExecutorService executor = Executors.newFixedThreadPool(numThreads);
		AtomicBoolean solutionFound = new AtomicBoolean(false);

		List<int[][]> startingBoards = generateStartingBoards(vals, partitionSize, numThreads);
		for (int[][] board : startingBoards) {
			executor.submit(() -> {
				ExactMatrix matrix = new ExactMatrix(board);
				Cell[][] finalMatrix = matrix.makeFinalMatrix();
				DancingLinkSolver solver = new DancingLinkSolver(finalMatrix, boardSize, solutionFound);
                if (solutionFound.get()) {
                    solutionMatrix.set(solver.solution); // Store the solution when found
                }
            });
        }

		shutdownAndAwaitTermination(executor);

		Sudoku.endTime = System.currentTimeMillis(); // Record end time
		System.out.println("Solved Sudoku:");
		System.out.print("Time taken: " + (Sudoku.endTime - Sudoku.startTime) + " milliseconds\n");

		int[][] finalSolution = solutionMatrix.get();
		printSudokuBoard(boardSize, partitionSize, finalSolution);
		printSudokuBoardInFile(boardSize, partitionSize, finalSolution);
	}
}
