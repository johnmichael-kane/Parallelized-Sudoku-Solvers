# Parallel-Sudoku-Project
# Dancing Link Approach

see the branch [DancingLinksUsingJava](https://github.com/johnmichael-kane/Parallelized-Sudoku-Solvers/tree/DancingLinksUsingJava) for a more complete and detailed implementation.
The current version here only works for 9x9, the branch above works for 9x9, 16x16, 25x25 implementation.

### Run
1. Enter the correct directory: `cd dancing-links`
2. Compile the DancingLinkSolver: `javac DancingLinkSolver.java`
3. Compile Sudoku.java: `javac Sudoku.java`
4. Run the file: `java Sudoku <FILE_NAME> <NUM_THREADS>`
- <FILE_NAME> accepts the current file names like 9x9_input.txt
- <NUM_THREADS> only accepts 1, 2, 4, and 8. 
