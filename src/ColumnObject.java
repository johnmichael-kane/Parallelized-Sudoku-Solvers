
public class ColumnObject extends DancingLinkObject {

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
