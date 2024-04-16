# Parallelized-Sudoku-Solvers

Below is an overview of three different approaches to solving a Sudoku board: 

(1) [Brute Force](#1-brute-force) (Java)

(2) [Dancing Links](#2-dancing-links) (Java)

(3) [Propagation-Cross-Search](#3-propagation-cross-search) (C++)

We have referenced sequential solutions of these techniques and created a parallelized implementation to test how performance changes with parallelism. 

A complete background and runtime analysis of each Sudoku solver is documented in our [report](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/attachments/Parallelized_Sudoku_Solvers.pdf) and [Google slides](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/attachments/Parallelized_Sudoku_Slides.pdf).

## Prerequisites

-   C++ compiler (g++) with C++11 support.
-   Git (for cloning the repository).
-   Java (for running the Brute Force and Dancing Links implementation).
-   Clang (for running the Propagate-Cross-Search implementation).

### 1. Check Prerequisite Installations

-   **GCC (g++):** `g++ --version`
-   **Git:** `git --version`
-   **Clang:** `clang --version`
-   **Java:** `java -version`

### 2. Install Prerequisites

-   **GCC (g++):**
    -   **Linux (Ubuntu/Debian):**
        ```sh
        sudo apt-get update
        sudo apt-get install g++
    -   **macOS:** `xcode-select --install`
    -   **Windows:** Install [MinGW](https://www.mingw-w64.org/) or [Cygwin](https://cygwin.com/) to get the g++ compiler.
-   **Git:**
    -   **Linux (Ubuntu/Debian):**
        ```sh
        sudo apt-get update
        sudo apt-get install git
    -   **macOS:**
        - Check Homebrew installation: `brew --version`
        - Install [Homebrew](https://brew.sh/) if not installed already.
        - Run: `brew install git`
    -   **Windows:** Install [Git for Windows](https://gitforwindows.org/).
-   **Java:**
    -   **Linux (Ubuntu/Debian):**
        ```sh
        sudo apt-get update
        sudo apt-get install default-jdk
    -   **macOS:**
        - Check Homebrew installation: `brew --version`
        - Install [Homebrew](https://brew.sh/) if not installed already.
        - Run: `brew install openjdk`
    -   **Windows:** Install [JDK for Oracle]([https://gitforwindows.org/](https://www.oracle.com/java/technologies/downloads/)) or [AdoptOpenJDK]([https://gitforwindows.org/](https://adoptium.net/)).
-   **Clang:**
    -   **Linux (Ubuntu/Debian):**
        ```sh
        sudo apt-get update
        sudo apt-get install clang
    -   **macOS:**
        - Check Homebrew installation: `brew --version`
        - Install [Homebrew](https://brew.sh/) if not installed already.
        - Run: `brew install llvm`
    -   **Windows:** Install [LLVNM](https://releases.llvm.org/download.html).

### 3. Clone Repository
```sh
git clone https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main
cd Parallelized-Sudoku-Solvers
```

## Project Overview

### Repository Features

- All files located in the 'main' branch.
- Each implementation uses the same puzzles.
- Each implementation implements a sequential approach that takes a file as input.
    - **Levels:** easy, medium, hard
    - **Sizes:** 9x9, 16x16, 25x25
- Each implementation implements a parallelized approach that take a file and thread number as input.

### Common Challenges

- Beating the Sudoku solver sequential efficiency with parallelism.
- Managing thread overhead, value synchronization, deadlocks, and thread conflicts.

# (1) Brute Force 
There are two implementations of the Brute Force technique: 
- Java [sequential](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/BruteForceUsingBacktracking) approach.
- Java [parallelized](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/ParallelizedBruteForce) appoach.
Code built off of a referenced [repository](https://github.com/bryanesmith/Sudoku-solver/blob/master/SudokuPuzzle.cpp).

# (2) Dancing Links
There are two implementations of the Dancing Links technique: 
- Java [sequential](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/dancing-links/basic) approach.
- Java [parallelized](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/dancing-links/parallelized) approach.
Code built off of a referenced [repository](https://github.com/gkaranikas/dancing-links/tree/master).

## Implementation

### Sequential

The sequential implementation of Sudoku solving using Dancing Links is based on Donald Knuth's algorithm for efficiently solving exact cover problems. In this implementation, the Sudoku puzzle is represented as an exact cover problem, where each constraint corresponds to a matrix row and each possible placement of a number corresponds to a matrix column.

The implementation for the sequential Dancing-Link contains the following files:
- [Cell.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/basic/Cell.java): Represents a cell in the Sudoku grid, storing its row, column, and assigned number.
- [ColumnObject.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/basic/ColumnObject.java): Extends DancingLinkObject and represents a column in the Dancing Links matrix. It maintains the size of the column and handles covering and uncovering operations.
- [DancingLinkObject.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/basic/DancingLinkObject.java): Represents a node in the Dancing Links matrix. It maintains references to adjacent nodes and the column it belongs to.
- [DancingLinkSolver.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/basic/DancingLinkSolver.java): Implements the Dancing Links algorithm to search for solutions recursively. It uses backtracking to explore possible placements of numbers while maintaining efficiency by choosing the column with the fewest possibilities first.
- [ExactMatrix.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/basic/ExactMatrix.java): Converts the given Sudoku grid into an exact cover matrix by representing each possible placement of a number as a linked matrix row. Each row corresponds to a cell in the Sudoku grid, and each column corresponds to a constraint (row, column, number, and grid).
- [Sudoku.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/basic/Sudoku.java): Reads the Sudoku puzzle from an input file, initializes the exact cover matrix, and utilizes the DancingLinkSolver to find and print the solution.

### Run 

1. Enter the correct directory: `cd dancing-links/basic`
2. Compile Sudoku.java: `javac Sudoku.java`
3. Run the file: `java Sudoku <FILE_NAME>`
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. View the [data](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/data) to see all inputs.

### Parallelized

Parallelization for dancing-links is a little complicated, as the actual process itself is very fast and efficient, and uses a lot of backtracking and public information. So a traditional parallel approach would be difficult to implement. Rather, the strategy for the parallelization involves an initial BFS (Breadth-First-Search) where the program will create n boards, which each belong to n threads. After each thread has a board assigned, then they'll go through the normal dancing-links solution indpendently and once a thread finds a solution, a flag is marked and all the threads return.

The implementation consists of two files that carry all of the work:
- [Sudoku.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/parallelized/Sudoku.java): Reads the Sudoku puzzle from an input file, generates starting boards using BFS, initializes multiple instances of the Dancing Links solver, and prints the solution.
- [DancingLinkSolver.java](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/blob/main/dancing-links/parallelized/DancingLinkSolver.java): Implements the Dancing Links algorithm with multithreading support. It creates a linked matrix representation of the Sudoku puzzle and performs parallel backtracking search to find solutions.

### Run

1. Enter the correct directory: `cd dancing-links/parallelized`
2. Compile the DancingLinkSolver: `javac DancingLinkSolver.java`
3. Compile Sudoku.java: `javac Sudoku.java`
4. Run the file: `java Sudoku <FILE_NAME> <NUM_THREADS>`
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. To see all inputs, view [Data](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/data)
- <NUM_THREADS> only accepts 1, 2, 4, and 8, representing each of the threads.

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
Time taken: 10 milliseconds
 
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
Time taken: 59 milliseconds
 
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
Time taken: 283 milliseconds
 
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

# (3) Propagation-Cross-Search
There are two implementations of the Propagation-Cross-Search technique: 
- C++ [sequential](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/PropagateCrossSearch/sequential/) approach.
- C++ [parallelized](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/PropagateCrossSearch/parallel/) approach.
Code built off of a referenced [repository](https://github.com/anthemEdge/Sudoku-Solver).

### Implementation

Technique:
1. Check linear constraints (stand-alone values).
2. Cross-reference rows, columns, and sections (intersecting values) if any remaining empty cells.
3. Depth first search if any remaining empty cells.

Parallelization Points:
- Game and PossibleGrid files.
- Conditionally parallelized DFS (on "hard" boards), parallelized possible and unsolved position checks.

### Sequential

![Demo Image](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/attachments/sequential-prop-cross-search)

### Parallelized

![Demo Image](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/attachments/parallel-prop-cross-search)

### Run

1. Open your terminal and navigate to the directory containing the repository (see [Clone Repository](#3-clone-repository)).
2. Navigate to the directory containing the propagation and search files (`cd PropagateCrossSearch`).
3. Navigate to the directory containing the implementation of your choice.
   
   a. Sequential: `cd sequential`

   b. Parralelized: `cd parallel`
   
5. Compile: `g++ -std=c++17 -g -o SudokuSolver SudokuSolver.cpp Game.cpp Grid.cpp PossibleGrid.cpp`
6. Run: `./SudokuSolver`
7. Replace "filename" in `filename.txt` with the board mode and size, then enter the number of threads (if in `parallel` directory) when prompted. Refer to the console for guidance.

# Troubleshooting

- Go back in the terminal using `cd ..` and forward using `cd [directory]`
- If you encounter any compilation errors, ensure you have the correct permissions and that your g++ installation is up to date with C++17 support.
- If you face issues with the pthread library on Windows, make sure you have the correct threading library for your setup or seek equivalent compilation flags for MinGW or Cygwin.
