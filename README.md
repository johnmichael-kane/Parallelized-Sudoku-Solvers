# Parallel-Sudoku-Project
# Dancing-Links Approach
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
//define strategy

# Implementation
//define implementation
### Run
1. Enter the correct directory: `cd dancing-links/parallelized`
2. Compile the DancingLinkSolver: `javac DancingLinkSolver.java`
3. Compile Sudoku.java: `javac Sudoku.java`
4. Run the file: `java Sudoku <FILE_NAME> <NUM_THREADS>`
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. To see all inputs, view [Data](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/main/data)
- <NUM_THREADS> only accepts 1, 2, 4, and 8, representing each of the threads.
