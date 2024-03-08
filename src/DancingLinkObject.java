
public class DancingLinkObject {

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
