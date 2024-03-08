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

## Testing

### 9x9 Grid Size Board

#### Input
```plaintext
| 03   .  04 |  .  06  09 |  .  05   . |
|  .   .   . | 02  07   . |  .   .  04 |
| 09   .  02 |  .   .  04 |  .   .   . |
 
|  .  02   . |  .  08  05 |  .  01  09 |
| 08   .  09 |  .   .   . | 02   .  05 |
| 05  01   . | 03  09   . |  .  06   . |
 
|  .   .   . | 08   .   . | 05   .  03 |
| 02   .   . |  .  04  06 |  .   .   . |
|  .  04   . | 07  05   . | 09   .  06 |

```
#### Output
```plaintext
Solved Sudoku:
Time taken: 1 milliseconds
 
| 03  07  04 | 01  06  09 | 08  05  02 |
| 06  05  01 | 02  07  08 | 03  09  04 |
| 09  08  02 | 05  03  04 | 06  07  01 |
 
| 04  02  03 | 06  08  05 | 07  01  09 |
| 08  06  09 | 04  01  07 | 02  03  05 |
| 05  01  07 | 03  09  02 | 04  06  08 |
 
| 07  09  06 | 08  02  01 | 05  04  03 |
| 02  03  05 | 09  04  06 | 01  08  07 |
| 01  04  08 | 07  05  03 | 09  02  06 |
```

### 16x16 Grid Size Board


#### Input
```plaintext
|  .  15   .  01 |  .  02  10  14 | 12   .   .   . |  .   .   .   . |
|  .  06  03  16 | 12   .  08  04 | 14  15  01   . | 02   .   .   . |
| 14   .  09  07 | 11  03  15   . |  .   .   .   . |  .   .   .   . |
| 04  13  02  12 |  .   .   .   . | 06   .   .   . |  .  15   .   . |
 
|  .   .   .   . | 14  01  11  07 | 03  05  10   . |  .  08   .  12 |
| 03  16   .   . | 02  04   .   . |  .  14  07  13 |  .   .  05  15 |
| 11   .  05   . |  .   .   .   . |  .  09  04   . |  .  06   .   . |
|  .   .   .   . | 13   .  16  05 | 15   .   .  12 |  .   .   .   . |
 
|  .   .   .   . | 09   .  01  12 |  .  08  03  10 | 11   .  15   . |
| 02  12   .  11 |  .   .  14  03 | 05  04   .   . |  .   .  09   . |
| 06  03   .  04 |  .   .  13   . |  .  11  09  01 |  .  12  16  02 |
|  .   .  10  09 |  .   .   .   . |  .   .  12   . | 08   .  06  07 |
 
| 12  08   .   . | 16   .   .  10 |  .  13   .   . |  .  05   .   . |
| 05   .   .   . | 03   .  04  06 |  .  01  15   . |  .   .   .   . |
|  .  09  01  06 |  .  14   .  11 |  .   .  02   . |  .   .  10  08 |
|  .  14   .   . |  .  13  09   . | 04  12  11  08 |  .   .  02   . |

```

#### Output
```plaintext
Solved Sudoku:
Time taken: 3723 milliseconds

| 08  15  11  01 | 06  02  10  14 | 12  07  13  03 | 16  09  04  05 |
| 10  06  03  16 | 12  05  08  04 | 14  15  01  09 | 02  11  07  13 |
| 14  05  09  07 | 11  03  15  13 | 08  02  16  04 | 12  10  01  06 |
| 04  13  02  12 | 01  09  07  16 | 06  10  05  11 | 03  15  08  14 |
 
| 09  02  06  15 | 14  01  11  07 | 03  05  10  16 | 04  08  13  12 |
| 03  16  12  08 | 02  04  06  09 | 11  14  07  13 | 10  01  05  15 |
| 11  10  05  13 | 08  12  03  15 | 01  09  04  02 | 07  06  14  16 |
| 01  04  07  14 | 13  10  16  05 | 15  06  08  12 | 09  02  03  11 |
 
| 13  07  16  05 | 09  06  01  12 | 02  08  03  10 | 11  14  15  04 |
| 02  12  08  11 | 07  16  14  03 | 05  04  06  15 | 01  13  09  10 |
| 06  03  14  04 | 10  15  13  08 | 07  11  09  01 | 05  12  16  02 |
| 15  01  10  09 | 04  11  05  02 | 13  16  12  14 | 08  03  06  07 |
 
| 12  08  04  03 | 16  07  02  10 | 09  13  14  06 | 15  05  11  01 |
| 05  11  13  02 | 03  08  04  06 | 10  01  15  07 | 14  16  12  09 |
| 07  09  01  06 | 15  14  12  11 | 16  03  02  05 | 13  04  10  08 |
| 16  14  15  10 | 05  13  09  01 | 04  12  11  08 | 06  07  02  03 |

```
