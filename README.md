# Parallel-Sudoku-Project

## Requirements: Milestone
- Finish the compilable code instructions in the readme file below
- Finish a rough draft of the paper
- All challenges, tasks, and goals for the project outlined

# Brute Force Approach
//general description of the algorithm and the idea behind it

### Run
//add run instructions here

# Mapping Approach
//general description of the algorithm and the idea behind it

### Run
//add run instructions here

# Dancing Link Approach

### Files
dancing_links.cpp 

### Run
1. Enter the correct directory: `cd "Dancing Links"`
2. Compile dancing_links and linked_matrix:
- `g++ -std=c++11 -c dancing_links.cpp -o dancing_links.o`
- `g++ -std=c++11 -c linked_matrix.cpp -o linked_matrix.o`
4. Enter the directory for the sudoku_tests, and compile it:
- `cd Test`
- `-std=c++11 -c sudoku_tests.cpp -o sudoku_tests.o`
5. Return to the "Dancing Links" directory and then finish compiling the sudoku_solver:
- `cd ..`
- `g++ -std=c++11 -o sudoku_solver dancing_links.o linked_matrix.o Test/sudoku_tests.o`
6. Run: `./sudoku_solver`

