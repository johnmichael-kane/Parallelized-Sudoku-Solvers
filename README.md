# Parallel-Sudoku-Project
# Sequential Dancing-Links
View the sequential java approach [here](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/dancing-links/basic).

The non-parallelized implementation of Sudoku solving using Dancing Links is based on Donald Knuth's algorithm for efficiently solving exact cover problems. In this implementation, the Sudoku puzzle is represented as an exact cover problem, where each constraint corresponds to a matrix row and each possible placement of a number corresponds to a matrix column.

# Implementation
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
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. To see all inputs, view [Data](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/data)

# Parallelized Dancing-Links
View the parallelized java approach [here](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/dancing-links/parallelized).

Parallelization for dancing-links is a little complicated, as the actual process itself is very fast and efficient, and uses a lot of backtracking and public information. So a traditional parallel approach would be difficult to implement. Rather, the strategy for the parallelization involves an initial BFS (Breadth-First-Search) where the program will create n boards, which each belong to n threads. After each thread has a board assigned, then they'll go through the normal dancing-links solution indpendently and once a thread finds a solution, a flag is marked and all the threads return.

# Implementation
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
