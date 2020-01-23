package il.co.ilrd.tests;

import java.io.FileNotFoundException;
import java.nio.file.Paths;

import il.co.ilrd.factory.FSTreeFactorized;

public class FSTreeFactorizedTest {
	public static void main(String[] args) {
		FSTreeFactorized tree = null;
		try {
			if(args.length == 0) {
				tree = new FSTreeFactorized(Paths.get("C://Users//varagor//Desktop/git/C").toString());					
			} else {
				tree = new FSTreeFactorized(args[0]);
			}
			
			tree.print();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
}
