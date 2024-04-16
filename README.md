# Parallel-Sudoku-Project
# Dancing-Links Approach

### Run 
1. Enter the correct directory: `cd dancing-links/basic`
2. Compile Sudoku.java: `javac Sudoku.java`
3. Run the file: `java Sudoku <FILE_NAME>`
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. To see all inputs, view [Data]()


# Parallelized Dancing-Links
View the java [DancingLinks]() approach.

see the branch [DancingLinksUsingJava](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/DancingLinksUsingJava) for a more complete and detailed implementation.
The current version here only works for 9x9, the branch above works for 9x9, 16x16, 25x25 implementation.

### Run
1. Enter the correct directory: `cd dancing-links/parallelized`
2. Compile the DancingLinkSolver: `javac DancingLinkSolver.java`
3. Compile Sudoku.java: `javac Sudoku.java`
4. Run the file: `java Sudoku <FILE_NAME> <NUM_THREADS>`
- <FILE_NAME> accepts different files for the 9x9, 16x16, and 25x25 of varying difficulties. To see all inputs, view [Data]()
- <NUM_THREADS> only accepts 1, 2, 4, and 8, representing each of the threads.
