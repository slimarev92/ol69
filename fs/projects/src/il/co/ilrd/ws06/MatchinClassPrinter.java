package il.co.ilrd.ws06;

import java.net.URLClassLoader;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.io.File;

public class MatchinClassPrinter {
	private static final int NUM_ARGS = 2;
	private String interName;
	private JarFile jFile;
	private URLClassLoader loader;

	public MatchinClassPrinter(String path, String interName) throws IOException {
		loader = new URLClassLoader(new URL[] {new File(path).toURI().toURL()});
		jFile = new JarFile(path);
		this.interName = interName;
	}

	private List<String> getClasses() {
		Enumeration<JarEntry> entries = jFile.entries();
		List<String> result = new ArrayList<String>();
		JarEntry curr = null;
				
		while(entries.hasMoreElements()) {
			curr = entries.nextElement();
			
			if(curr.getName().endsWith(".class")) {
				String curStr = curr.getName().replaceAll("/", ".").replaceAll(".class", "");
				
				result.add(curStr);
			} 
		}
		
		return result;
	}	
		
	public void printMatchingClasses() {
		List<String> classes = getClasses();
		
		System.out.println("Total num of classes " + classes.size());
		System.out.println("Classes that implement " + interName + ":");
		
		for(String currStr : classes) {
			Class<?> currClass = null;
			
			try {
				currClass = loader.loadClass(currStr);
				
				for(Class<?> inter : currClass.getInterfaces()) {
					System.out.println(currClass.getName());
					if(inter.getName().equals(interName)) {
					}
				}
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void main(String[] args) {
		if(NUM_ARGS > args.length) {
			throw new IllegalArgumentException();
		}
		
		try {
			MatchinClassPrinter loader = new MatchinClassPrinter(args[0], args[1]);
			loader.printMatchingClasses();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}