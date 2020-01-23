package il.co.ilrd.exam;

public class Q1B {
	
	/* Q1 - B */
	public static void moveZeroesRight(int[] array) {
		int left = 0;
		int right = array.length - 1;
			
		while(left < right) {
			if(0 == array[right]) {
				--right;
			} else if(0 != array[left]) {
				++left;
			} else {
				array[left]  = array[right];
				array[right] = 0;
			}
		}
	}
	
	public static void main(String[] args) {
		int[] arr = {0, 9, 8, 4, 0, 0, 2, 7, 0, 6, 0};
		
		System.out.print("Before: ");
		
		for(int i = 0; i < arr.length; ++i) {
			System.out.print(arr[i] + " ");
		}
		
		System.out.print("\nAfter: ");
		
		Q1B.moveZeroesRight(arr);
		
		for(int i = 0; i < arr.length; ++i) {
			System.out.print(arr[i] + " ");
		}
		
		System.out.println();
		
	}
}
