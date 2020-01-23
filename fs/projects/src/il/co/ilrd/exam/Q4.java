package il.co.ilrd.exam;

class BST {
	Integer[] tree;
	private static final int INITIAL_SIZE = 8;
	private static final int ROOT_INDEX = 1;
	
	public BST(int root) {
		tree = new Integer[INITIAL_SIZE];
		tree[0] = 0xDEADC0DE;
		tree[ROOT_INDEX] = root;
	}
	
	public static int leftChildIndex(int index) {
		return index * 2;
	}
	
	public static int rightChildIndex(int index) {
		return (index * 2) + 1;
	}
	
	private boolean indexWithinBounds(int index) {
		return index < tree.length - 1;
	}
	
	private void increaseArraySize() {
		Integer[] temp = new Integer[tree.length * 2];
		
		for(int i = 0; i < tree.length; ++i) {
			temp[i] = tree[i];
		}
		
		tree = temp;
	}
	
	public void insert(int value) {
		int i = ROOT_INDEX;
		
		while(true) {
			if(!indexWithinBounds(i)) {
				increaseArraySize();
			}
			
			if(null == tree[i]) {
				tree[i] = value;
				return;
			}
						
			i = value > tree[i] ? BST.rightChildIndex(i) : BST.leftChildIndex(i);				
		}
	}
	
	public void printLevel(int level) {
		int max = getLevelStart(level);
		
		for(int i = 0; i < max; ++i) {
			if(null != tree[i + max]) { 
				System.out.print(tree[i + max] + " ");				
			}
		}
	}
		
	private int getMaxLevel() {
		int curr = (int)tree.length;
		int count = 0;
		
		while(1 < curr) {
			++count;
			curr /= 2;
		}
		
		return count - 1;
	}
	
	private int getLevelStart(int level) {
		return (int)Math.pow(2.0, level * 1.0);
	}
	
	public void print() {
		for(int i = 0; i < tree.length; ++i) {
			System.out.println("i = " + i + " value = " + tree[i]);
		}
	}
	
	public void printLevels() {
		int maxLevel = getMaxLevel();
			
		for(int i = maxLevel; 0 <= i; --i) {
			printLevel(i);
		}
	}
	
	public void printLevelsReverse() {
		int maxLevel = getMaxLevel();
		
		for(int i = 0; maxLevel >= i; ++i) {
			printLevel(i);
		}
	}
}

public class Q4 {
	public static void main(String[] args) {
		BST a = new BST(20);
		
		a.insert(10);
		a.insert(30);
		a.insert(8);
		a.insert(15);
		a.insert(25);
		a.insert(40);
		a.insert(7);
		
		a.printLevels();
		System.out.println();
		a.printLevelsReverse();


	}
}
