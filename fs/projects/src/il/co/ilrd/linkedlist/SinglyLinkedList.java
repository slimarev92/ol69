package il.co.ilrd.linkedlist;

public class SinglyLinkedList {
	private Node head;

	public SinglyLinkedList() {
		head = null;
	}
	
	public SinglyLinkedList(Object data) {
		head = new Node(data, null);
	}

	public void pushFront(Object data) {
		head = new Node(data, head);
	}
	
	public int size() {
		Node currNode = head;
		int counter = 0;
		
		while(null != currNode) {
			++counter;
			currNode = currNode.next;
		}
	
		return counter;
	}
	
	public void popFront() {
		head = head.next;
	}
	
	public boolean isEmpty() {
		return null == head;
	}

	public Iterator begin () {
		return new NodeIterator(head);
	}

	public Iterator find(Object target) {
		Iterator curr = begin();
		Iterator prev = null;
		
		while(curr.hasNext()) {
			prev = curr;
			
			if(curr.next().equals(target)) {
				return prev;
			}		
		}
		
		return null;
	}
	
	private class NodeIterator implements Iterator {
		private Node node;

		public NodeIterator(Node node) {
			this.node = node;
		}

		@Override
		public boolean hasNext() {
			return null != node;
		}

		@Override
		public Object next() {
			Object currData = node.data;
			node = node.next;
			return currData;
		}
	}
	
	private class Node {
		private Object data;
		private Node next;

		public Node(Object data, Node next) {
			this.data = data;
			this.next = next;
		}
	}
}
