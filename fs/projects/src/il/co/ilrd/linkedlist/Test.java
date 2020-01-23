package il.co.ilrd.linkedlist;

public class Test {

	public static void main(String[] args) {
		SinglyLinkedList list = new SinglyLinkedList();
		Iterator iter = null;
		
		System.out.println(list.isEmpty());
		
		list.pushFront("One");
		list.pushFront("Two");
		list.pushFront(3);
		list.pushFront(4.0);
		
		iter = list.begin();
		
		while(iter.hasNext()) {
			System.out.println(iter.next());
		}
		
		System.out.println(list.size());
		
		iter = list.find(4.0);
		
		System.out.println(iter.next());
	}
	
}
