package il.co.ilrd.composite;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;

public class FSTree {
	private Root dir;
	private static final String LAYER_STRING = "   ";
	private static final String ANGLE = "└──"; 
	 
	public FSTree(String path) throws FileNotFoundException {
		File file = new File(path);
		
		if(!file.isDirectory() || 0 == file.list().length) {
			throw new FileNotFoundException("Given path is not a directory");
		}
		
		dir = new Root(file);
	}
	
	private interface FSNode {
		public void print();
	}
	
	private class Leaf implements FSNode {
		private File file;
		private int depth;
		
		public Leaf(File file, int depth) {
			this.file = file;
			this.depth = depth;
		}
		
		public void print() {
			for(int i = 0; i < depth; ++i) {
				System.out.print(LAYER_STRING);
			}
			
			System.out.println(ANGLE + file.getName());
		}
	}
	
	private class Root implements FSNode {
		private File file;
		private List<FSNode> nodes = new ArrayList<FSNode>();
		private int depth;
	
		public Root(File file) {	
			this.file = file;
			depth = 0;
			
			generateNodeList();
		}
		
		public Root(File file, int depth) {
			this.file = file;
			this.depth = depth;
			
			generateNodeList();
		}
		
		private void generateNodeList() {
			File[] nodeList = file.listFiles();
				
			Arrays.sort(nodeList);
					
			for(File curr : nodeList) {			
				if(curr.isDirectory() && 0 < curr.list().length) {
					nodes.add(new Root(curr, depth + 1));
				} else {
					nodes.add(new Leaf(curr, depth + 1));
				}
			}
		}
		
		public void print() {
			
			for(int i = 0; depth > i; ++i) {
				System.out.print(LAYER_STRING);
			}
			
			System.out.println(ANGLE + file.getName());
					
			for(FSNode node : nodes) {
				node.print();
			}
		}
	}
	
	public void print() {
		dir.print();
	}
}