import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;

public class SudokuSolver implements Runnable {
    private int[][] board;
    private final int gridSize;
    private final int blockSize;
    private static AtomicBoolean solved = new AtomicBoolean(false);
    private static long startTime;
    private static long endTime;

    public SudokuSolver(int[][] board) {
        this.board = board;
        this.gridSize = board.length;
        this.blockSize = (int) Math.sqrt(gridSize);
    }

    public void run() {
        solve(0, 0);
    }

    private boolean solve(int row, int col) {
        if (solved.get()) return true;
        if (row == gridSize) {
            solved.set(true);
            return true;
        }

        int nextRow = col == gridSize - 1 ? row + 1 : row;
        int nextCol = col == gridSize - 1 ? 0 : col + 1;

        if (board[row][col] != 0) {
            return solve(nextRow, nextCol);
        }

        for (int num = 1; num <= gridSize; num++) {
            if (isValidPlacement(row, col, num)) {
                board[row][col] = num;
                if (solve(nextRow, nextCol)) {
                    return true;
                }
                board[row][col] = 0;
            }
        }

        return false;
    }

    private boolean isValidPlacement(int row, int col, int number) {
        return !isNumberInRow(row, number) && !isNumberInColumn(col, number) && !isNumberInBox(row, col, number);
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
        int localBoxRow = row - row % blockSize;
        int localBoxCol = col - col % blockSize;

        for (int i = localBoxRow; i < localBoxRow + blockSize; i++) {
            for (int j = localBoxCol; j < localBoxCol + blockSize; j++) {
                if (board[i][j] == number) {
                    return true;
                }
            }
        }
        return false;
    }

    // Method to print the solved Sudoku board to a file
    public void printSudokuBoardInFile(String level, int threadsNum) {
        try {
            FileWriter writer = new FileWriter(gridSize + "x" + gridSize + "_" + level + "_w_"+threadsNum+"_thread_out.txt"); // Open file for writing
            writer.write("Time taken: " + (endTime - startTime) + " milliseconds\n"); // Write time taken to solve
            writer.write("Number of Threads: "+threadsNum+"\n");
            writer.write("Solved Sudoku:\n");
            for (int y = 0; y < gridSize; y++) {
                if (y % blockSize == 0) {
                    writer.write("\n");
                }
                for (int x = 0; x < gridSize; x++) {
                    if (x % blockSize == 0) {
                        writer.write("|");
                    }
                    writer.write(board[y][x] == 0 ? "  . " : " " + formatNumber(board[y][x]) + " ");
                }
                writer.write("|\n");
            }
            writer.close(); // Close the writer after writing
        } catch (IOException e) {
            e.printStackTrace(); // Handle IO exception
        }
    }

    private String formatNumber(int number) {
        return number > 9 ? Integer.toString(number) : "0" + number;
    }

    public static void main(String[] args) {
        if (args.length < 3) {
            System.out.println("Usage: java SudokuSolver <filePath> <Level> <numberOfThreads> ");
            return;
        }
        String filePath = args[0];
        int numThreads = Integer.parseInt(args[2]);
        String level =args[1];

        int[][] board = readSudokuFile(filePath);
        if (board == null) return;

        ExecutorService executor = Executors.newFixedThreadPool(numThreads);
        startTime = System.currentTimeMillis();

        for (int i = 0; i < numThreads; i++) {
            executor.execute(new SudokuSolver(board));
        }

        executor.shutdown();

        while (!executor.isTerminated()) {
            // wait for all tasks to finish
        }

        endTime = System.currentTimeMillis();

        if (solved.get()) {
            System.out.println("Solved Sudoku:");
            new SudokuSolver(board).printSudokuBoardInFile(level,numThreads);
            System.out.println(("Number of Threads: "+numThreads));
            System.out.println("Time taken: " + (endTime - startTime) + " milliseconds");
            printSudokuBoard(board);
        } else {
            System.out.println("No solution exists.");
        }
    }

    private static int[][] readSudokuFile(String filePath) {
        try (Scanner scanner = new Scanner(new File(filePath))) {
            int size = scanner.nextInt();
            int[][] board = new int[size][size];

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (scanner.hasNextInt()) {
                        board[i][j] = scanner.nextInt();
                    }
                }
            }
            return board;
        } catch (FileNotFoundException e) {
            System.err.println("File not found");
            e.printStackTrace();
            return null;
        }
    }

    private static void printSudokuBoard(int[][] board) {
        int size = board.length;
        int blockSize = (int) Math.sqrt(size);

        for (int y = 0; y < size; y++) {
            if (y % blockSize == 0) {
                System.out.println(" ");
            }
            for (int x = 0; x < size; x++) {
                if (x % blockSize == 0) {
                    System.out.print("|");
                }
                System.out.print(board[y][x] == 0 ? " . " : (board[y][x] > 9 ? " " + board[y][x] : " 0" + board[y][x]) + " ");
            }
            System.out.println("|");
        }
        System.out.println("");
    }
}
