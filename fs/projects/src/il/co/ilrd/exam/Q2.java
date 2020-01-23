package il.co.ilrd.exam;

class SimpleListNode {
	int value;
	SimpleListNode next;
	
	public SimpleListNode(int value) {
		this.value = value;
		this.next = null;
	}
	
	public void insertAtEnd(SimpleListNode next) {
		SimpleListNode curr = this;
		
		while(null != curr.getNext()) {
			curr = curr.getNext();
		}
		
		curr.setNext(next);
	}
	
	public void insertAtEnd(int value) {
		SimpleListNode curr = this;
		
		while(null != curr.getNext()) {
			curr = curr.getNext();
		}
		
		curr.setNext(value);
	}
	
	public SimpleListNode getNext() {
		return next;
	}
	
	public int getValue() {
		return value;
	}

	public void setNext(int value) {
		next = new SimpleListNode(value);
	}
	
	public void setNext(SimpleListNode next) {
		this.next = next;
	}
	
	public String toString() {
		return value + " ";
	}
	
	public void printList() {
		SimpleListNode node = this;
		
		while(null != node) {
			System.out.print(node.getValue() + "->");
			node = node.getNext();
		}
		
		System.out.println();
	}
	
	public SimpleListNode findLoop() {
		SimpleListNode slow = this;
		SimpleListNode fast = getNext();
		
		while(null != fast && slow != fast) {
			slow = slow.getNext();
			fast = fast.getNext();
			
			if(null != fast) {
				fast = fast.getNext();
			}
		}
		
		return fast;
	}
	
	public void openLoop() {
		SimpleListNode inTheLoop = findLoop();
	
		SimpleListNode curr = inTheLoop;
		SimpleListNode endCandidate = this;
		
		while(curr.getNext() != endCandidate) {		
			if(curr.getNext() == inTheLoop) {
				endCandidate = endCandidate.getNext();
			} 
			
			curr = curr.getNext();
		}
		
		curr.setNext(null);
	}
	
	public int size() {
		int result = 0;
		SimpleListNode curr = this;
		
		while(null != curr) {
			++result;
			curr = curr.getNext();
		}
	
		return result;
	}
	
	public SimpleListNode findIntersection(SimpleListNode otherList) {
		SimpleListNode shorter = this;
		SimpleListNode longer = otherList;
		int lenShort = size();
		int lenLong = otherList.size();
		
		if(lenShort >  lenLong) {
			shorter = otherList;
			longer = this;
			
			int temp = lenShort;
			lenShort = lenLong;
			lenLong = temp;
		} 

		for(int i = 0; lenLong - lenShort > i; ++i) {
			longer = longer.getNext();
		}
		
		while(longer.getNext() != shorter.getNext()) {
				shorter = shorter.getNext();
				longer = longer.getNext();
		}
		
		return longer.getNext();
	}
	
	public void separateIntersection(SimpleListNode otherList) {
		SimpleListNode intersection = findIntersection(otherList);
		SimpleListNode curr = this;
		
		while(curr.getNext() != intersection) {
			curr = curr.getNext();
		}
		
		curr.setNext(null);
	}
 }

public class Q2 {
	@SuppressWarnings("unused")
	public static void main(String[] args) {
		//PART A
		SimpleListNode list = new SimpleListNode(10);
		SimpleListNode loopEnd = new SimpleListNode(30);
		SimpleListNode loopStart = new SimpleListNode(100);
		
		list.insertAtEnd(20);
		list.insertAtEnd(loopEnd);

		
		loopEnd.setNext(loopEnd);
		
		list.openLoop();
		
		list.printList();

		//PART B
		System.out.println("---------------------------------------------");
		SimpleListNode intersection = new SimpleListNode(50);
		list = new SimpleListNode(10);
		list.insertAtEnd(20);
		list.insertAtEnd(30);
		list.insertAtEnd(40);
		list.insertAtEnd(intersection);
		
		SimpleListNode list2 = new SimpleListNode(30);
		list2.insertAtEnd(40);
		list2.insertAtEnd(intersection);
		
		System.out.print("List 1 before: ");
		list.printList();
		
		System.out.print("List 2 before: ");
		list2.printList();
		
		list.separateIntersection(list2);
		
		System.out.print("List 1 after: ");
		list.printList();
		
		System.out.print("List 2 after: ");
		list2.printList();
	}
}
