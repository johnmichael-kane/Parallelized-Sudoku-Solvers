import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

public class DancingLinkSolver {
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
