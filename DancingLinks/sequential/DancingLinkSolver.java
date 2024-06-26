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
	}

	public void startSearch() {
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
		// printMatrix(headers);
	}

	private void printMatrix(List<ColumnObject> headers) {
		// System.out.println("Complete Matrix:");
		for (ColumnObject colHead : headers) {
			// System.out.print("Column " + colHead.name + ": ");
			DancingLinkObject temp = colHead.down;
			while (temp != colHead) {
				// System.out.print("(" + temp.info.row + "," + temp.info.col + "," +
				// temp.info.number + ") ");
				temp = temp.down;
			}
			// System.out.println(); // Newline for the next column
		}
	}

	public void search() {
		// System.out.println(String.format("header: %s %s", header.size, header.name));
		// System.out.println(String.format("header: %s %s %s", header.info.row,
		// header.info.col, header.info.number));
		// System.out.println(String.format("header->right: %s %s %s",
		// header.right.info.row, header.right.info.col, header.right.info.number));
		if (header.right == header) {
			// System.out.println(String.format("header: %s %s %s", header.info.row,
			// header.info.col, header.info.number));
			// System.out.println(String.format("header->right: %s %s %s",
			// header.right.info.row, header.right.info.col, header.right.info.number));
			makeSolution();
		} else {
			ColumnObject curr = chooseColumn();
			curr.cover();
			for (DancingLinkObject r = curr.down; r != curr; r = r.down) {
				// System.out.println(String.format("r %s %s %s", r.info.row, r.info.col,
				// r.info.number));
				result.push(r);
				for (DancingLinkObject j = r.right; j != r; j = j.right) {
					j.column.cover();
				}
				search();
				if (done) {
					break;
				}
				r = result.pop();
				curr = r.column;
				for (DancingLinkObject j = r.left; j != r; j = j.left) {
					j.column.uncover();
				}
			}

			curr.uncover();
		}
	}

	// brings the print to main
	public int[][] getSolution() {
		return solution;
	}

	private void makeSolution() {
		done = true;
		while (!result.isEmpty()) {
			DancingLinkObject curr = result.pop();
			solution[curr.info.row][curr.info.col] = curr.info.number;
		}
		// for (int i = 0; i < solution.length; i++) {
		// for (int j = 0; j < solution.length; j++) {
		// System.out.print(solution[i][j]);
		// }
		// System.out.println();
		// }
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
