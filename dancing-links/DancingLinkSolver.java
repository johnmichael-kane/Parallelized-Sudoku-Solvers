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
	int[][] solution;
	public ColumnObject header;
	int size;
	Stack<DancingLinkObject> result;
	private AtomicBoolean solutionFound;

	public DancingLinkSolver(Cell[][] matrix, int size, AtomicBoolean solutionFound) {
		this.size = size;
		this.solution = new int[size][size];
		result = new Stack<>();
		this.header=makeLinks(matrix);
		this.solutionFound = solutionFound;
		search();
	}

	private ColumnObject makeLinks(Cell[][] matrix) {
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
		return head;
	}

	private void search() {
		if (solutionFound.get()) return; //Check if another thread found the solution

		if (header.right == header) {
			makeSolution();
			return;
		} 

		ColumnObject curr = chooseColumn();
		curr.cover();
		for (DancingLinkObject r = curr.down; r != curr; r = r.down) {
			result.push(r);
			for (DancingLinkObject j = r.right; j != r; j = j.right) {
				j.column.cover();
			}
			
			search(); 
			if (solutionFound.get()) break; 
			
			r = result.pop();
			curr = r.column;
			for (DancingLinkObject j = r.left; j != r; j = j.left) {
				j.column.uncover();
			}
		}
		curr.uncover();	
	}
	
	//for some reason with the comments it runs 100% of the time but not without them
	private void makeSolution() {
		DancingLinkObject curr = result.pop();
		int i=0;
		if(curr.info.number != 0){
			solution[curr.info.row][curr.info.col] = curr.info.number;
			System.out.print(curr.info.number);
			while (!result.isEmpty()) {
				if(curr.info.number == 0) return;
				curr = result.pop();
				solution[curr.info.row][curr.info.col] = curr.info.number;
				System.out.print(curr.info.number);
			}
			System.out.println("\ni: " + i);
			solutionFound.set(true);
		}
		else{
			System.out.println("INVALID!");
		}
		
	}

	private ColumnObject chooseColumn() {
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