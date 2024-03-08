## Sudoku Solver using Backtracking

This program implements a Sudoku solver using a brute-force approach with backtracking.

### How to Run

To execute the solver, follow these steps:

1. Compile the Java code:
    ```sh
    javac Sudoku.java
    ```

2. Run the compiled program with a Sudoku puzzle file as an argument. Replace `<FILE_NAME>` with the name of the puzzle file:
    ```sh
    java Sudoku <FILE_NAME>
    ```

### Understanding the Code

The `Sudoku` class encapsulates the functionality for reading Sudoku puzzles from files, solving them using backtracking, and printing the solutions. Here's an overview of how it works:

1. **Imports**: The code imports necessary classes for file handling and scanning.

2. **Class Declaration**: The `Sudoku` class represents a Sudoku puzzle.

3. **Instance Variables**: The class contains instance variables for the Sudoku board, grid size, block size, and timing.

4. **Constructor**: The constructor initializes the Sudoku puzzle by reading it from a file.

5. **Methods**: The class provides methods for reading the puzzle file, checking the validity of number placements, solving the puzzle recursively, formatting numbers, and printing the Sudoku board.

6. **Main Method**: The main method reads the file path from command-line arguments, initializes a `Sudoku` object, prints the initial Sudoku board, solves the puzzle, prints the solved Sudoku board to the console, and writes it to a file if a solution is found.

The solver employs a backtracking algorithm to efficiently solve Sudoku puzzles by recursively trying different number placements and backtracking when necessary.

### Testing the Code

