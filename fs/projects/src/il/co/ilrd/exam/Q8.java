package il.co.ilrd.exam;

import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

class FirstNonDuplicate {
	Dequeue<Integer> nonDups = new Dequeue<>();
	Map<Integer, SelfRemoveableNode> numsMap = new HashMap<>();
	
	public void receiveNum(int num) {	
		if(!numsMap.containsKey(num)) {
			numsMap.put(num, nonDups.insert(num));		
		} else {
			numsMap.get(num).remove();
		}	
	}
	
	public int firstNonDup() {
		Integer data = nonDups.peek();

		return null == data ? 0 : data;
	}
	
	private interface SelfRemoveableNode {
		public void remove();	
	}

	private static class Dequeue<T> {
		private Node<T> head;
		private Node<T> tail;

		public Dequeue() {
			head = new Node<>(null);
			tail = new Node<>(null);

			head.setNext(tail);
			tail.setPrev(head);
		}

		public Node<T> insert(T data) {
			Node<T> currPrev = tail.getPrev();

			tail.setPrev(data);
			currPrev.setNext(tail.getPrev());
			tail.getPrev().setPrev(currPrev);

			return tail.getPrev();
		}

		@SuppressWarnings("unused")
		public int size() {
			Node<T> curr = head;
			int res = 0;

			while (null != curr) {
				++res;
				curr = curr.getNext();
			}

			return res;
		}

		@SuppressWarnings("unused")
		public void print() {
			Node<T> curr = head.getNext();

			while (null != curr) {
				curr = curr.getNext();
			}

			System.out.println();
		}

		public T peek() {
			return null == head.getNext() ? null : head.getNext().getData();
		}

		@SuppressWarnings("unused")
		public boolean isEmpty() {
			return null == head;
		}

		private static class Node<T> implements SelfRemoveableNode {
			T data;
			Node<T> next;
			Node<T> prev;

			public Node(T data) {
				this.data = data;
			}

			public Node(T data, Node<T> prev) {
				this.data = data;
				this.prev = prev;
			}

			@SuppressWarnings("unused")
			public void setNext(T nextData) {
				next = new Node<>(nextData, this);
			}

			public void setPrev(T prevData) {
				prev = new Node<>(prevData);
				prev.setNext(this);
			}

			public void setNext(Node<T> next) {
				this.next = next;
			}

			public void setPrev(Node<T> prev) {
				this.prev = prev;
			}

			public Node<T> getNext() {
				return next;
			}

			public Node<T> getPrev() {
				return prev;
			}

			public T getData() {
				return data;
			}

			@Override
			public void remove() {
				Node<T> currPrev = this.prev;
				Node<T> currNext = this.next;

				if (null != currPrev) {
					currPrev.next = currNext;
				}

				if (null != currNext) {
					currNext.prev = currPrev;
				}
			}

			@Override
			public String toString() {
				return data.toString();
			}
		}
	}
}

public class Q8 {
	public static void main(String[] args) {
		Integer[] input = {1, 2, 2, 3, 3, 4, 1, 1, 1, 5, 4};
		List<Integer> output = new ArrayList<>();
		FirstNonDuplicate nd = new FirstNonDuplicate();
		
		System.out.print("t ");
		
		for(int i = 1; i <= 11; ++i) {
			System.out.printf("%02d ", i);
		}
		
		System.out.print("\ni ");
		
		for(Integer curr : input) {
			System.out.printf("%02d ", curr);
			nd.receiveNum(curr);
			output.add(nd.firstNonDup());
		}
		
		System.out.print("\no ");
		
		for(Integer curr : output) {
			System.out.printf("%02d ", curr);
		}
	}
}
