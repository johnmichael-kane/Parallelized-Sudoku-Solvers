# Parallel-Sudoku-Project
# Sequential Dancing-Links
View the sequential java approach [here](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/dancing-links/basic).

//define strategy

# Implementation
//define implementaiton

### Run 
1. Enter the correct directory: `cd dancing-links/basic`
2. Compile Sudoku.java: `javac Sudoku.java`
3. Run the file: `java Sudoku <FILE_NAME>`
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. To see all inputs, view [Data](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/data)

# Parallelized Dancing-Links
View the parallelized java approach [here](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/dancing-links/parallelized).

Parallelization for dancing-links is a little complicated, as the actual process itself is very fast and efficient, and uses a lot of backtracking and public information. So a traditional parallel approach would be difficult to implement. Rather, the strategy for the parallelization involves an initial BFS (Breadth-First-Search) where the program will create n boards, which each belong to n threads. After each thread has a board assigned, then they'll go through the normal dancing-links solution indpendently and once a thread finds a solution, a flag is marked and all the threads return.

# Implementation
//define implementation

### Run
1. Enter the correct directory: `cd dancing-links/parallelized`
2. Compile the DancingLinkSolver: `javac DancingLinkSolver.java`
3. Compile Sudoku.java: `javac Sudoku.java`
4. Run the file: `java Sudoku <FILE_NAME> <NUM_THREADS>`
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. To see all inputs, view [Data](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/data)
- <NUM_THREADS> only accepts 1, 2, 4, and 8, representing each of the threads.
