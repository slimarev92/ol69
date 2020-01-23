package il.co.ilrd.exam;

class SimpleStack {
	int curr;
	int[] data;
	
	public SimpleStack(int capacity) {
		data = new int[capacity];
		curr = 0;
	}
	
	public int getCapacity() {
		return data.length;
	}
	
	public int getSize() {
		return curr;
	}
	
	public boolean isFull() {
		return curr >= data.length;
	}
	
	public void push(int value) {
		data[curr]  = value;
		++curr;
	}
	
	public int pop() {
		--curr;
		int ret = data[curr];
		data[curr] = 0;
		return ret;
	}
	
	public int peek() {
		return data[curr - 1];
	}
	
	public void printStack() {
		for(int i = data.length - 1; 0 <= i; --i) {
			System.out.print(data[i] + " ");
		}
		
		System.out.println();
	}
}

class Sort {
	public static void sortStack(SimpleStack stack) {
		SimpleStack from = stack;
		SimpleStack to = new SimpleStack(from.getCapacity());
		
		for(int i = 0; i < to.getCapacity() - 1; ++i) {
			Sort.pushLargestInPlace(to, from, Sort.filterLargest(from, to, i));
		}
	}
	
	public static void pushLargestInPlace(SimpleStack from, SimpleStack to, int largest) {
		int max = from.getSize() + 1;
	
		for(int i = 0, toPush = 0; i < max; ++i) {	
			toPush = (i != 0) ? from.pop() : largest;
			
			to.push(toPush);	
		}
	}

	public static int filterLargest(SimpleStack from, SimpleStack to, int iter) {
		int largest = from.pop();
		
		for(int i = 0; i < from.getCapacity() - iter - 1; ++i) {	
			int curr = from.pop();
					
			if(curr > largest) {
				to.push(largest);
				largest = curr;
			} else {
				to.push(curr);
			}
		}
	
		return largest;
	}
}

class PrintReverse {
	private static String[] split(String str) {
		return str.split(" ");
	}
	
	private static void printWordReverse(String word) {
		for(int i = word.length() - 1; 0 <= i; --i) {
			System.out.print(word.charAt(i));
		}
	}
	
	public static void printWordsReveresed(String str) {
		for(String word : PrintReverse.split(str)) {
			PrintReverse.printWordReverse(word);
			System.out.print(" ");
		}
		
		System.out.println();
	}
}

public class Q3 {
	public static void main(String[] args) {
		//PART 1
		PrintReverse.printWordsReveresed("Hello my name is jimmy pop");
		
		System.out.println("--------------------------------------");
		
		//PART 2
		SimpleStack stack = new SimpleStack(6);
		
		stack.push(3);
		stack.push(23);
		stack.push(31);
		stack.push(98);
		stack.push(92);
		stack.push(34);
	
		stack.printStack();
		Sort.sortStack(stack);
		stack.printStack();
	}
}
