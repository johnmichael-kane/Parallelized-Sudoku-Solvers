public class ExactMatrix {
	
	public int[][] matrix;
	public Cell[][] finalMatrix;
	int sudokuSize;
	
	public ExactMatrix(int[][] matrix) {
		this.matrix = matrix;
		finalMatrix = new Cell[matrix.length*matrix.length*matrix.length][matrix[0].length*matrix[0].length*4];
		sudokuSize = this.matrix.length;
		initMatrix();
		makeFinalMatrix();
//		printMatrix();
	}
	
	private void initMatrix() {
		for (int i=0;i<finalMatrix.length; i++) {
			for (int j=0;j<finalMatrix[0].length; j++){
				finalMatrix[i][j] = null;
			}
		}
		
	}

	public void makeFinalMatrix() {
		for (int i=0;i<sudokuSize; i++) {
			for (int j=0;j<sudokuSize; j++){
				int currNum = matrix[i][j];
				fillInMatrix(i,j,currNum);
			}
		}
	}

	private void fillInMatrix(int row, int col, int number) {
		for (int i = 1; i <= sudokuSize; i++) {
				if (number!=0){
					fillCell(row, col, number);
				} else {
					for (int num=1; num<=sudokuSize; num++) {
						fillCell(row, col, num);
					}
				}
		}
	}

	private void fillCell(int row, int col, int number) {
		int matrixRow = row*matrix.length*matrix.length + col*matrix.length+number-1;
		int col1 = row*sudokuSize+col;
		int col2 = sudokuSize*sudokuSize+row*sudokuSize+number-1;
		int col3 = 2*sudokuSize*sudokuSize+col*sudokuSize+number-1;
		int col4 = 3*sudokuSize*sudokuSize+getGridNumber(row, col)*sudokuSize+number-1;
		finalMatrix[matrixRow][col1] = new Cell(row, col, number);
		finalMatrix[matrixRow][col2] = new Cell(row, col, number);
		finalMatrix[matrixRow][col3] = new Cell(row, col, number);
		finalMatrix[matrixRow][col4] = new Cell(row, col, number);
	}

	private int getGridNumber(int row, int col) {

		int gridSize = (int)Math.sqrt(sudokuSize);

		return (col/gridSize)+((int)(row/gridSize))*gridSize;
	}
	

}
