import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

class Cell {
	public int row;
	public int col;
	public int number;

	public Cell(int row, int col, int number) {
		this.row = row;
		this.col = col;
		this.number = number;
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
		left = right = up = down = this;
	}

	public DancingLinkObject(ColumnObject column) {
		this();
		this.column = column;
	}

	public DancingLinkObject(ColumnObject column, Cell info) {
		this(column);
		this.info = info;
	}

	public void horizontalInsert(DancingLinkObject toAdd) {
		toAdd.left = this;
		this.right.left = toAdd;
		toAdd.right = this.right;
		this.right = toAdd;
	}

	public void verticalInsert(DancingLinkObject toAdd) {
		toAdd.up = this;
		this.down.up = toAdd;
		toAdd.down = this.down;
		this.down = toAdd;
	}
}

class ColumnObject extends DancingLinkObject {

	public int size;
	public int name;

	public ColumnObject(int i) {
		super();
		size = 0;
		column = this;
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
	private AtomicReference<int[][]> solutionMatrix;

	public DancingLinkSolver(Cell[][] matrix, int size, AtomicBoolean solutionFound,
			AtomicReference<int[][]> solutionMatrix) {
		this.size = size;
		this.solution = new int[size][size];
		this.solutionMatrix = solutionMatrix;

		this.result = new Stack<>();
		this.header = makeLinks(matrix);
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

	private boolean isCompleteSolution() {
		if (result.size() != size * size) {
			// System.out.println("nah");
			return false;
		}
		int i = 0;

		boolean[][] seen = new boolean[size][size];
		for (DancingLinkObject item : result) {
			// System.out.print(Thread.currentThread().getName() + ": loop"+i+"/" + (size *
			// size) + ":\t");
			// System.out.println(item.info.number);
			if (item.info == null || seen[item.info.row][item.info.col] || item.info.number == 0) {
				// If we encounter a null cell info, or we've already seen a cell for this row
				// and column,
				// it's not a complete solution.
				// System.out.println("ERRRR!");
				return false;
			}
			seen[item.info.row][item.info.col] = true;
			i++;
		}

		// System.out.println("total i: " + i);
		// If we've gotten this far, every cell has been seen exactly once, so it's a
		// complete solution.
		return true;
	}

	private void search() {
		if (solutionFound.get())
			return; // Check if another thread found the solution

		if (header.right == header) {
			// System.out.println("Thread " + Thread.currentThread().getName() + " is a
			// complete solution?");
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
			if (solutionFound.get())
				break;

			r = result.pop();
			curr = r.column;
			for (DancingLinkObject j = r.left; j != r; j = j.left) {
				j.column.uncover();
			}
		}
		curr.uncover();
	}

	// for some reason with the comments it runs 100% of the time but not without
	// them
	private void makeSolution() {
		synchronized (solutionFound) {
			if (!solutionFound.get() && isCompleteSolution()) {
				int[][] newSolution = new int[size][size];
				for (DancingLinkObject dlo : result) {
					newSolution[dlo.info.row][dlo.info.col] = dlo.info.number;
				}
				solutionMatrix.set(newSolution);
				solutionFound.set(true);
			} else {
				// System.out.println(Thread.currentThread().getName() + "is NOT a real working
				// solution!");
			}
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