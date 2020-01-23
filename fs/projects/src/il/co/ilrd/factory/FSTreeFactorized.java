package il.co.ilrd.factory;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;
import il.co.ilrd.factory.Factory;

public class FSTreeFactorized {
	private FSNode dir;
	private static final String LAYER_STRING = "   ";
	private static final String ANGLE = "└──"; 
	 
	public FSTreeFactorized(String path) throws FileNotFoundException {
		File file = new File(path);
		
		if(!file.isDirectory() || 0 == file.list().length) {
			throw new FileNotFoundException("Given path is not a directory");
		}
		
		dir = NodeFactory.manufacture(NodeType.ROOT, file);
	}
	
	
	public void print() {
		dir.print(0);
	}
	
	private interface FSNode {
		public void print(int depth);
	}
	
	private static class Leaf implements FSNode {
		private File file;
		
		public Leaf(File file) {
			this.file = file;
		}
		
		public void print(int depth) {
			for(int i = 0; i < depth; ++i) {
				System.out.print(LAYER_STRING);
			}
			
			System.out.println(ANGLE + file.getName());
		}
	} //class Leaf
	
	private static class Root implements FSNode {
		private File file;
		private List<FSNode> nodes;
				
		public Root(File file) {
			this.file = file;
			nodes = new ArrayList<FSNode>();
			
			generateNodeList();
		}
				
		private void generateNodeList() {
			File[] nodeList = file.listFiles();
				
			Arrays.sort(nodeList);
					
			for(File curr : nodeList) {		
				nodes.add(NodeFactory.manufacture(getNodeType(curr), curr));
			}
		}
		
		private NodeType getNodeType(File file) {
			return (file.isDirectory() && 0 < file.list().length) ? NodeType.ROOT : NodeType.LEAF;
		}
		
		public void print(int depth) {
			for(int i = 0; depth > i; ++i) {
				System.out.print(LAYER_STRING);
			}
			
			System.out.println(ANGLE + file.getName());
					
			for(FSNode node : nodes) {
				node.print(depth + 1);
			}
		}
	} //class Root
	
	private static class NodeFactory {
		private static final Factory<NodeType, File, FSNode> factory;
		
		static {
			factory = new Factory<>();
			factory.add(NodeType.LEAF, Leaf::new);
			factory.add(NodeType.ROOT, Root::new);
		}
		
		private static FSNode manufacture(NodeType type, File file) {
			return factory.create(type, file);
		}
	} //class NodeFactory
	
	private enum NodeType { LEAF, ROOT; }
}