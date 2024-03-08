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
    java Sudoku <FILE_NAME> <Level of boead (ex. easy, med, hard)>
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

## Testing for Easy Boead

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

### 25x25 Grid Size Board

#### Input
```plaintext
 
|  .   .   .  08  15 |  .  21  06  17   . |  .   .   .   .   . | 24  19   .   .   . | 12   .   .   .  02 |
|  .   .   .  03  11 |  .  16  02  04  13 | 07  25  08  17   . | 01  15  20  18  22 | 06  19  09  10  05 |
|  .   .   .  04  07 | 14   .   .   .   . |  .   .  01   .   . | 08   .  03   .   . |  .  11   .  22  24 |
|  .   .  17  06  02 | 23  01   .  12  11 |  .  18   .   .  04 | 10  05  16  09   . | 13  08  07  15  03 |
| 10   .  09   .  13 |  .  08  15   .   . | 24   .   .   .  06 |  .   .  11  12   . |  .  18  04   .  20 |
 
| 06  03  02   .   . | 15  25  01  14  16 | 23  05  04   .  09 | 22  24  18  08   . | 19  12  13  07  17 |
|  .   .   .   .   . |  .   .   .  09   . | 12  07   .  06   . |  .  14  04  02  17 | 16  05  03  11  10 |
|  .   .   .  12   . | 06   .  03   .  04 |  .  14   .  10   . | 07   .  01  15  05 | 09  21  02  24  08 |
|  .   .  05  14   . |  .  07   .   .  02 |  .  08   .   .   . | 03   .  09  25  16 | 15  06  01  04  18 |
| 09  04  07   .   . | 08  05   .  10  17 | 03  01  02  18   . |  .  12  06   .  19 | 20  22  14  23  25 |
 
| 18   .   .   .   . |  .   .   .  23   . | 02  10   .  05   . |  .  03   .  21  01 |  .   .   .   .   . |
| 03  23  21  02  05 | 20  06  17  15  01 |  .  04   .   .   . |  .   .   .   .  07 |  .  09   .   .  13 |
| 04   .  06   .  01 | 11  02  18  25  05 | 09   .  24  07  08 | 17  13  12  23  15 | 10  16  19  03   . |
| 08  09  13   .   . | 07  19  21  24  10 | 01  20   .  23  03 | 16  06  02  14  11 | 22  04  17  05  15 |
|  .   .   .   .   . | 16   .   .  03  08 | 06  21  25  15   . | 04  18   .  05  09 |  .   .   .   .   . |
 
| 13  02  19  16  20 | 05   .  07  08  22 | 10  17  14  04  12 | 15  09  25  03  24 | 21  01  18  06  11 |
| 07  12   .   .   . |  .   .   .  01  14 | 20  23   .  13   . |  .   .  17  19  06 |  .  15  25  09  16 |
| 11  06  10  17  04 | 09  03   .  02  25 | 05   .   .  08   . | 18  20  13  01  21 | 07  23  24  14  22 |
| 01  05  03  23  09 | 21  17  19  16  15 |  .  06  11  24  18 | 14  08  07  22  12 | 02  13  10  20  04 |
| 25  15  18  22  14 | 13  04  24  06   . | 21  03  07  09  01 | 11  02  23  16  10 | 08  17  05  12  19 |
 
| 02  11  01  07  25 | 10  15  08  13  12 | 18  19   .   .   . | 06   .  05  04  03 | 17  24  22  16  09 |
| 17  08  12  13  21 | 02   .  25  07  06 | 04  24  09  01  05 | 19  16  15   .   . | 11  03  20  18  23 |
| 15  10  22  09  06 | 03  14  04  20  24 | 16   .  23  25  17 | 12  01  21  11  18 | 05  02  08   .  07 |
| 05   .   .   .   . | 17  09  16  11  23 |  .   .   .   .   . |  .  07  24  20   . |  .   .  06  21  01 |
| 14   .  16  24  23 | 01  18  05  21  19 | 08  11  06  03  07 | 09  25  22  17  02 | 04  10  15  13   . |

```

#### Output
```plaintext

Solved Sudoku:
Time taken: 16202 milliseconds
 
| 23  18  20  08  15 | 22  21  06  17  03 | 13  09  05  11  10 | 24  19  14  07  04 | 12  25  16  01  02 |
| 12  21  14  03  11 | 24  16  02  04  13 | 07  25  08  17  23 | 01  15  20  18  22 | 06  19  09  10  05 |
| 19  16  25  04  07 | 14  10  09  05  18 | 15  02  01  12  20 | 08  17  03  06  13 | 23  11  21  22  24 |
| 22  24  17  06  02 | 23  01  20  12  11 | 14  18  21  19  04 | 10  05  16  09  25 | 13  08  07  15  03 |
| 10  01  09  05  13 | 25  08  15  19  07 | 24  16  03  22  06 | 02  21  11  12  23 | 14  18  04  17  20 |
 
| 06  03  02  11  10 | 15  25  01  14  16 | 23  05  04  21  09 | 22  24  18  08  20 | 19  12  13  07  17 |
| 20  25  08  01  18 | 19  24  13  09  21 | 12  07  15  06  22 | 23  14  04  02  17 | 16  05  03  11  10 |
| 16  13  23  12  22 | 06  20  03  18  04 | 17  14  19  10  25 | 07  11  01  15  05 | 09  21  02  24  08 |
| 21  17  05  14  19 | 12  07  23  22  02 | 11  08  13  20  24 | 03  10  09  25  16 | 15  06  01  04  18 |
| 09  04  07  15  24 | 08  05  11  10  17 | 03  01  02  18  16 | 21  12  06  13  19 | 20  22  14  23  25 |
 
| 18  22  15  19  16 | 04  13  14  23  09 | 02  10  17  05  11 | 20  03  08  21  01 | 24  07  12  25  06 |
| 03  23  21  02  05 | 20  06  17  15  01 | 19  04  12  16  14 | 25  22  10  24  07 | 18  09  11  08  13 |
| 04  14  06  20  01 | 11  02  18  25  05 | 09  22  24  07  08 | 17  13  12  23  15 | 10  16  19  03  21 |
| 08  09  13  25  12 | 07  19  21  24  10 | 01  20  18  23  03 | 16  06  02  14  11 | 22  04  17  05  15 |
| 24  07  11  10  17 | 16  12  22  03  08 | 06  21  25  15  13 | 04  18  19  05  09 | 01  20  23  02  14 |
 
| 13  02  19  16  20 | 05  23  07  08  22 | 10  17  14  04  12 | 15  09  25  03  24 | 21  01  18  06  11 |
| 07  12  24  21  08 | 18  11  10  01  14 | 20  23  22  13  02 | 05  04  17  19  06 | 03  15  25  09  16 |
| 11  06  10  17  04 | 09  03  12  02  25 | 05  15  16  08  19 | 18  20  13  01  21 | 07  23  24  14  22 |
| 01  05  03  23  09 | 21  17  19  16  15 | 25  06  11  24  18 | 14  08  07  22  12 | 02  13  10  20  04 |
| 25  15  18  22  14 | 13  04  24  06  20 | 21  03  07  09  01 | 11  02  23  16  10 | 08  17  05  12  19 |
 
| 02  11  01  07  25 | 10  15  08  13  12 | 18  19  20  14  21 | 06  23  05  04  03 | 17  24  22  16  09 |
| 17  08  12  13  21 | 02  22  25  07  06 | 04  24  09  01  05 | 19  16  15  10  14 | 11  03  20  18  23 |
| 15  10  22  09  06 | 03  14  04  20  24 | 16  13  23  25  17 | 12  01  21  11  18 | 05  02  08  19  07 |
| 05  19  04  18  03 | 17  09  16  11  23 | 22  12  10  02  15 | 13  07  24  20  08 | 25  14  06  21  01 |
| 14  20  16  24  23 | 01  18  05  21  19 | 08  11  06  03  07 | 09  25  22  17  02 | 04  10  15  13  12 |

````

