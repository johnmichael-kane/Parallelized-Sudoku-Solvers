public class DancingLinkSolver2Thread extends DancingLinkSolver{

	public DancingLinkSolver2Thread(Cell[][] matrix, int size) {
        super(matrix,size);
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
	}
}