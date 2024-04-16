# Parallelized-Sudoku-Solvers

Below is an overview of three different approaches to solving a Sudoku board: 

(1) [Brute Force](#1-brute-force) (Java)

(2) [Dancing Links](#2-dancing-links) (Java)

(3) [Propagation and Search](#3-propagation-and-search-c-1) (C++)

We have referenced sequential solutions of these techniques and created a parallelized implementation to test how performance changes with parallelism. 

A complete background and runtime analysis of each Sudoku solver is documented in our [LaTeX report](https://docs.google.com/document/d/1aVoKkxx--F2Nfqbh13ZVF21VNVL2wHYg11CZaXs5kX8/edit?usp=sharing).

## Requirements: Milestone
- Finish the compilable code instructions in the readme file below
- Finish a rough draft of the paper
- All challenges, tasks, and goals for the project outlined

*************************** ADD INFO FOR JAVA ***************************
## Prerequisites

-   C++ compiler (g++) with C++11 support.
-   Git (for cloning the repository).

### 1. Check Prerequisite Installations

-   **GCC (g++):** `g++ --version`
-   **Git:** `git --version`
-   CLANG
-   JAVA

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
    -   **Windows:**
        - Download and install [Git for Windows](https://gitforwindows.org/).

### 3. Clone Repository
```sh
git clone https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main
cd Parallelized-Sudoku-Solvers
```

# Repository Overview
- shared file system
- shared repository
- shared parallelism approach (1, 2, 4, & 8 threads)

# (1) Brute Force 
View the Java [BruteForceUsingBacktracking](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/BruteForceUsingBacktracking) approach.

### Run
1.

# (2) Dancing Links
View the Java [BruteForceUsingBacktracking](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/BruteForceUsingBacktracking) approach.

This program implements a Sudoku solver using a brute-force approach with Dancing Links

### How to Run

To execute the solver, follow these steps:

1. Enter the correct directory:
    ```sh
    cd dancing-links
    ```

2. Compile the Java code:
    ```sh
    javac Sudoku.java
    ```

3. Run the compiled program with a Sudoku puzzle file as an argument. Replace `<filename>` with the name of the puzzle file:
    ```sh
    java Sudoku <version> <filename>
    ```
    - Acceptable <version> includes {'4threads', '8threads', and 'none'} for each of the types of parallelization. 
    - Acceptable <filename> includes {'9x9_input.txt', '16x16_input.txt', and '25x25_input.txt'} for each board size.
    

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

# (3) Propagation and Search
View the C++ [BruteForceUsingBacktracking](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/BruteForceUsingBacktracking) approach.

// describe strategy

### Implementation
// briefly describe & compare both implementations, highlighting your parallelized features, and link the original source here

### Demo

![Demo Image]()

### Run

1. Open your terminal and navigate to the directory containing the repository (see [Clone Repository](#3-clone-repository)).
2. Navigate to the directory containing the propagation and search files (`cd propagation-and-search`).
3. Navigate to the directory containing the implementation of your choice.
   a. Sequential: `cd sequential-prop-search`
   b. Parralelized: `cd parallelized-prop-search`
5. Compile: `g++ -std=c++11 -g -o SudokuSolver SudokuSolver.cpp Game.cpp Grid.cpp PossibleGrid.cpp`
6. Run: `./SudokuSolver`
7. Replace "filename" in `filename.txt` with the board mode and size, then enter the number of threads when prompted. Refer to the console for guidance.

# Troubleshooting

- If you encounter any compilation errors, ensure you have the correct permissions and that your g++ installation is up to date with C++11 support.
- If you face issues with the pthread library on Windows, make sure you have the correct threading library for your setup or seek equivalent compilation flags for MinGW or Cygwin.
