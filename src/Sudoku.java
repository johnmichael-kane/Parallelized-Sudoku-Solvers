import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;


public class Sudoku {
    private int[][] board;
    private int gridSize;
    private int blocksize;
    private long startTime;
    private long endTime;

    public Sudoku(String filePath) {
        readSudokuFile(filePath);
        this.blocksize = (int) Math.sqrt(gridSize);
    }

    private void readSudokuFile(String filePath) {
        try {
            Scanner scanner = new Scanner(new File(filePath));
            this.gridSize = scanner.nextInt(); // Read the first line for grid size
            this.board = new int[gridSize][gridSize];

            for (int i = 0; i < gridSize; i++) {
                for (int j = 0; j < gridSize; j++) {
                    if (scanner.hasNextInt()) {
                        board[i][j] = scanner.nextInt();
                    }
                }
            }
            scanner.close();
        } catch (FileNotFoundException e) {
            System.err.println("File not found");
            e.printStackTrace();
        }
    }

    private boolean isNumberInRow(int row, int number) {
        for (int i = 0; i < gridSize; i++) {
            if (board[row][i] == number) {
                return true;
            }
        }
        return false;
    }

    private boolean isNumberInColumn(int col, int number) {
        for (int i = 0; i < gridSize; i++) {
            if (board[i][col] == number) {
                return true;
            }
        }
        return false;
    }

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

    private boolean isValidPlacement(int row, int col, int number) {
        return !isNumberInRow(row, number) && !isNumberInColumn(col, number) && !isNumberInBox(row, col, number);
    }

    public boolean solve() {
        for (int row = 0; row < gridSize; row++) {
            for (int col = 0; col < gridSize; col++) {
                if (board[row][col] == 0) {
                    for (int number = 1; number <= gridSize; number++) {
                        if (isValidPlacement(row, col, number)) {
                            board[row][col] = number;
                            if (solve()) {
                                return true;
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
    public String n(int n){

        return n > 9 ? "" + n: "0" + n;
    }

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

    public void printSudokuBoardInFile() {
        try {
            FileWriter writer = new FileWriter("solution.txt");
            writer.write("Time taken: " + (endTime - startTime) + " milliseconds\n");
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
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        String filePath = args[0];
        Sudoku sudoku = new Sudoku(filePath);
        System.out.println("Sudoku Board:");
        sudoku.printSudokuBoard();
        sudoku.startTime = System.currentTimeMillis();
        if (sudoku.solve()) {
            sudoku.endTime = System.currentTimeMillis();
            System.out.println("Solved Sudoku:");
            System.out.print("Time taken: " + (sudoku.endTime - sudoku.startTime) + " milliseconds\n");
            sudoku.printSudokuBoard();
            sudoku.printSudokuBoardInFile();
        } else {
            System.out.println("No solution exists.");
        }
    }
}
