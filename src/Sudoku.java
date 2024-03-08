import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

// Class for solving Sudoku puzzles
public class Sudoku {
    private int[][] board; // 2D array representing the Sudoku board
    private int gridSize; // Size of the Sudoku grid
    private int blocksize; // Size of each block within the grid
    private long startTime; // Start time for solving the puzzle
    private long endTime; // End time for solving the puzzle

    // Constructor to initialize Sudoku puzzle from a file
    public Sudoku(String filePath) {
        readSudokuFile(filePath); // Read the Sudoku puzzle from the file
        this.blocksize = (int) Math.sqrt(gridSize); // Calculate the block size based on the grid size
    }

    // Method to read Sudoku puzzle from a file
    private void readSudokuFile(String filePath) {
        try {
            Scanner scanner = new Scanner(new File(filePath)); // Open the file for reading
            this.gridSize = scanner.nextInt(); // Read the first line for grid size
            this.board = new int[gridSize][gridSize]; // Initialize the Sudoku board array

            // Loop through the file contents to populate the Sudoku board
            for (int i = 0; i < gridSize; i++) {
                for (int j = 0; j < gridSize; j++) {
                    if (scanner.hasNextInt()) {
                        board[i][j] = scanner.nextInt(); // Read each integer from the file and assign it to the board
                    }
                }
            }
            scanner.close(); // Close the scanner after reading
        } catch (FileNotFoundException e) {
            System.err.println("File not found"); // Handle file not found
            e.printStackTrace();
        }
    }

    // Method to check if a number is already present in the specified row
    private boolean isNumberInRow(int row, int number) {
        for (int i = 0; i < gridSize; i++) {
            if (board[row][i] == number) {
                return true;
            }
        }
        return false;
    }

    // Method to check if a number is already present in the specified column
    private boolean isNumberInColumn(int col, int number) {
        for (int i = 0; i < gridSize; i++) {
            if (board[i][col] == number) {
                return true;
            }
        }
        return false;
    }

    // Method to check if a number is already present in the specified box
    private boolean isNumberInBox(int row, int col, int number) {
        int localBoxRow = row - row % blocksize;
        int localBoxCol = col - col % blocksize;

        for (int i = localBoxRow; i < localBoxRow + blocksize; i++) {
            for (int j = localBoxCol; j < localBoxCol + blocksize; j++) {
                if (board[i][j] == number) {
                    return true;
                }
            }
        }
        return false;
    }

    // Method to check if a number placement is valid in the Sudoku puzzle
    private boolean isValidPlacement(int row, int col, int number) {
        return !isNumberInRow(row, number) && !isNumberInColumn(col, number) && !isNumberInBox(row, col, number);
    }

    // Method to solve the Sudoku puzzle recursively using backtracking
    public boolean solve() {
        for (int row = 0; row < gridSize; row++) {
            for (int col = 0; col < gridSize; col++) {
                if (board[row][col] == 0) {
                    for (int number = 1; number <= gridSize; number++) {
                        if (isValidPlacement(row, col, number)) {
                            board[row][col] = number; // Place the number on the board
                            if (solve()) {
                                return true; // If the puzzle is solved, return true
                            } else {
                                board[row][col] = 0; // Reset and try next number
                            }
                        }
                    }
                    return false; // No valid number found, backtrack
                }
            }
        }
        return true; // Solved
    }

    // Method to format single-digit numbers with leading zero
    public String n(int n) {
        return n > 9 ? "" + n : "0" + n;
    }

    // Method to print the Sudoku board to the console
    public void printSudokuBoard() {
        for (int y = 0; y < gridSize; y++) {
            if (y % blocksize == 0) {
                System.out.println(" ");
            }

            for (int x = 0; x < gridSize; x++) {
                if (x % blocksize == 0) {
                    System.out.print("|");
                }
                System.out.print(board[y][x] == 0 ? "  . " : " " + n(board[y][x]) + " ");
            }
            System.out.println("|");
        }
        System.out.println("");
    }

    // Method to print the solved Sudoku board to a file
    public void printSudokuBoardInFile() {
        try {
            FileWriter writer = new FileWriter(gridSize+"x"+gridSize+"_out.txt"); // Open file for writing
            writer.write("Time taken: " + (endTime - startTime) + " milliseconds\n"); // Write time taken to solve
            writer.write("Solved Sudoku:\n");
            for (int y = 0; y < gridSize; y++) {
                if (y % blocksize == 0) {
                    writer.write("\n");
                }
                for (int x = 0; x < gridSize; x++) {
                    if (x % blocksize == 0) {
                        writer.write("|");
                    }
                    writer.write(board[y][x] == 0 ? "  . " : " " + n(board[y][x]) + " ");
                }
                writer.write("|\n");
            }
            writer.close(); // Close the writer after writing
        } catch (IOException e) {
            e.printStackTrace(); // Handle IO exception
        }
    }

    // Main method to run the Sudoku solver
    public static void main(String[] args) {
        String filePath = args[0]; // Get file path from command line argument
        Sudoku sudoku = new Sudoku(filePath);
        System.out.println("Sudoku Board:");
        sudoku.printSudokuBoard(); // Print initial Sudoku board
        sudoku.startTime = System.currentTimeMillis(); // Record start time
        if (sudoku.solve()) {
            sudoku.endTime = System.currentTimeMillis(); // Record end time
            System.out.println("Solved Sudoku:");
            System.out.print("Time taken: " + (sudoku.endTime - sudoku.startTime) + " milliseconds\n");
            sudoku.printSudokuBoard(); // Print solved Sudoku board to console
            sudoku.printSudokuBoardInFile(); // Write solved Sudoku board to file
        } else {
            System.out.println("No solution exists."); // Print message if no solution exists
        }
    }
}
