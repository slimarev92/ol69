package il.co.ilrd.tests;

import java.io.FileNotFoundException;

import il.co.ilrd.composite.FSTree;

public class FSTreeTest {
	public static void main(String[] args) {
		FSTree tree = null;
		try {
			if(args.length == 0) {
				tree = new FSTree("/home/sasha/Desktop/");					
			} else {
				tree = new FSTree(args[0]);
			}
			
			tree.print();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
	}
}
