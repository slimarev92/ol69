package il.co.ilrd.ws09;
 
class FooReference<T> {
	T reference;
	
	public FooReference(T reference) {
		this.reference = reference;
	}
	
	public T getReference() {
		return reference;
	}
	
	public void setReference(T reference) {
		this.reference = reference;
	}
}

public class GenTest {
	public static <T> void printArray(T[] arr) {
		for(T curr : arr) {
			System.out.println(curr);
		}
	}

	public static void main(String[] args) {
		String[] arr = {"Hello", "my", "name", "is jimmy pop"};
		Integer[] arr2 = {1, 2, 3, 4, 5};
		Character[] arr3 = {'h', 'e', 'l', 'l', 'o'};
		
		GenTest.printArray(arr);
		System.out.println("-----------");
		GenTest.printArray(arr2);
		System.out.println("-----------");
		GenTest.printArray(arr3);
		System.out.println("-----------");
		
		String a = "hello my name is jimmy pop";
		Integer b = 23;
		Character c = '@';
		
		FooReference<String> aRef = new FooReference<>(a);
		FooReference<Integer> bRef = new FooReference<>(b);
		FooReference<?> cRef = new FooReference<>(c);
	
		
		System.out.println(aRef.getReference());
		System.out.println("-----------");
		System.out.println(bRef.getReference());
		System.out.println("-----------");
		System.out.println(cRef.getReference());
		System.out.println("-----------");
		
		System.out.println(aRef.getClass().getName());
		System.out.println(bRef.getClass().getName());
		System.out.println(cRef.getClass().getName());

		
		System.out.println("<<<<<<<<<<<<<<<<<");
		@SuppressWarnings({ "rawtypes", "unchecked", "unused" })
		FooReference dRef = new FooReference(new Object());
		System.out.println("<<<<<<<<<<<<<<<<<");
	}
}
