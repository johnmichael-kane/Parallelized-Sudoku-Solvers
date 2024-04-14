import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;


class Cell {
	public int row;
	public int col;
	public int number;
	
	public Cell(int row, int col, int number) {
		this.row=row;
		this.col=col;
		this.number=number;
	}
}

class DancingLinkObject {

	public DancingLinkObject left;
	public DancingLinkObject right;
	public DancingLinkObject up;
	public DancingLinkObject down;
	public ColumnObject column;
	Cell info;
	
	public DancingLinkObject() {
		left=right=up=down=this;
	}

	public DancingLinkObject(ColumnObject column) {
		this();
		this.column=column;
	}
	public DancingLinkObject(ColumnObject column, Cell info) {
		this(column);
		this.info=info;
	}

	public void horizontalInsert(DancingLinkObject toAdd) {
		toAdd.left=this;
		this.right.left=toAdd;
		toAdd.right=this.right;
		this.right=toAdd;
	}

	public void verticalInsert(DancingLinkObject toAdd) {
		toAdd.up=this;
		this.down.up=toAdd;
		toAdd.down=this.down;
		this.down=toAdd;
	}
}

class ColumnObject extends DancingLinkObject {

	public int size;
	public int name;

	public ColumnObject(int i) {
		super();
		size=0;
		column=this;
		this.name = i;
	}

	public void cover() {
		this.right.left = this.left;
		this.left.right = this.right;
		for (DancingLinkObject i = this.down; i != this; i = i.down) {
			for (DancingLinkObject j = i.right; j != i; j = j.right) {
				j.down.up = j.up;
				j.up.down = j.down;
				j.column.size--;
			}
		}
	}

	public void uncover() {
		for (DancingLinkObject i = this.up; i != this; i = i.up) {
			for (DancingLinkObject j = i.left; j != i; j = j.left) {
				j.column.size++;
				j.down.up = j;
				j.up.down = j;
			}
		}
		this.right.left = this;
		this.left.right = this;
	}
}

public class DancingLinkSolver {
	
	//private Cell[][] initialBoard;
	//private ExecutorService executor;
	//private AtomicBoolean solutionFound = new AtomicBoolean(false);
	//private final int MAX_DEPTH = 20; //example depth for BFS phase
	//private volatile Solution finalSolution = null; //placeholder for actual solution implementaiton

	
	int[][] solution;
	public ColumnObject header;
	int size;
	Stack<DancingLinkObject> result;
	boolean done = false;

	

	public DancingLinkSolver(Cell[][] matrix, int size) {
		this.size = size;
		result = new Stack<>();
		solution = new int[size][size];
		makeLinks(matrix);
		search();
	}

	private void makeLinks(Cell[][] matrix) {
		ColumnObject head = new ColumnObject(-1);
		List<ColumnObject> headers = new ArrayList<>();

		for (int i = 0; i < matrix[0].length; i++) {
			ColumnObject currHeader = new ColumnObject(i);
			headers.add(currHeader);
			head.horizontalInsert(currHeader);
			head = currHeader;
		}
		head = head.right.column;

		for (int i = 0; i < matrix.length; i++) {
			DancingLinkObject prev = null;
			for (int j = 0; j < matrix[0].length; j++) {
				if (matrix[i][j] != null) {
					// if (j<10) {
					// System.out.println(String.format("[Column %s, (%s, %s:
					// %s)])", j, matrix[i][j].row, matrix[i][j].col,
					// matrix[i][j].number));
					// }
					ColumnObject columnHeader = headers.get(j);
					DancingLinkObject currNode = new DancingLinkObject(columnHeader, matrix[i][j]);
					if (prev == null) {
						prev = currNode;
					}
					columnHeader.up.verticalInsert(currNode);
					prev.horizontalInsert(currNode);
					prev = currNode;
					columnHeader.size++;
				}
			}

		}
		head.size = matrix[0].length;
		this.header = head;
	}

	public void search() {
		if (header.right == header) {
			makeSolution();
		} else {
			ColumnObject curr = chooseColumn();
			curr.cover();
			for (DancingLinkObject r = curr.down; r != curr; r = r.down) {
				result.push(r);
				for (DancingLinkObject j = r.right; j != r; j = j.right) {
					j.column.cover();
				}
				search();
				if (done)
					break;
				r = result.pop();
				curr = r.column;
				for (DancingLinkObject j = r.left; j != r; j = j.left) {
					j.column.uncover();
				}
			}
			curr.uncover();
		}
	}
	/* 
	public void solve(){
		List<Cell[][]> startingBoards = generateStartingBoards(this.initialBoard);

		for(Cell [][] board : startingBoards){
			if(solutionFound.get()) break;

			executor.submit(() -> solveBoard(board));
		}

		try{
			executor.shutdown();
			executor.awaitTermination(1, TimeUnit.HOURS);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		if(finalSolution != null){
			// output or process the solution
		} else{
			//handle the case where no solution is found
		}
	}

	//method to be BFS
	private List<Cell[][]> generateStartingBoards(Cell[][] board){
		//implement logic to generate all possible boards filling up to MAX_DEPTH empty spaces
		//This part simulates the baord generation kernal (in a GPU based implementaiton)
		return new ArrayList<>(); //return a list of potential board configurations
	}

	//simulated backtracking search for each board
	private void solveBoard(Cell[][] board){
		//implement DFS logic
		//adapted for parallel execution (without recursion)
		//this simulates backtracking kernel in GPU context

		if(/*checkIfSolution(board) ){
			solutionFound.set(true);
			//finalSolution = /*extractSolution(board) ;
			executor.shutdownNow(); //stop all running tasks immediately if it works
		}
	}
	*/
	private void makeSolution() {
		done = true;
		while (!result.isEmpty()) {
			DancingLinkObject curr = result.pop();
			solution[curr.info.row][curr.info.col] = curr.info.number;
		}
//		for (int i = 0; i < solution.length; i++) {
//			for (int j = 0; j < solution.length; j++) {
//				System.out.print(solution[i][j]);
//			}
//			System.out.println();
//		}
	}

	public ColumnObject chooseColumn() {
		ColumnObject toReturn = null;
		int min = Integer.MAX_VALUE;
		for (ColumnObject j = (ColumnObject) header.right; j != header; j = (ColumnObject) j.right) {
			if (j.size < min) {
				toReturn = j;
				min = j.size;
			}
		}
		return toReturn;
	}

}