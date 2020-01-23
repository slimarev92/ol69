package il.co.ilrd.ws03;

public class ClassInfoDumper {
	private Class<?> targetClass;

	public ClassInfoDumper(String className) throws ClassNotFoundException {
		targetClass = Class.forName(className);
	}

	public void dumpClassData() {

		PrintInfo("Package: ", targetClass.getPackage().getName());
		PrintInfo("Name", targetClass.getName());
		PrintInfo("Annotation", targetClass.getAnnotations());
		PrintInfo("Public Methods: ", targetClass.getCanonicalName());
		PrintInfo("Classes: ", targetClass.getClasses());
		PrintInfo("Constructors: ", targetClass.getConstructors());
		PrintInfo("Public Fields: ", targetClass.getFields());
		PrintInfo("Interfaces: ", targetClass.getInterfaces());
		PrintInfo("Public Methods: ", targetClass.getMethods());
	}

	private void PrintInfo(String title, String info) {
		System.out.println(title + ": " + info);
		System.out.println("---------------------");
	}
	
	private void PrintInfo(String title, Object[] arr) {
		System.out.println(title + ": ");
		ClassInfoDumper.PrintArr(arr);
		System.out.println("--------------------");
	}
	
	private static void PrintArr(Object[] arr) {
		
		for(Object element : arr) {
			System.out.println(element);
		}
	}
	
	public static void main(String[] args) {

		ClassInfoDumper dumper = null;

		try {
			dumper = new ClassInfoDumper("il.co.ilrd.linkedlist.SinglyLinkedList");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}

		dumper.dumpClassData();
	}
}
