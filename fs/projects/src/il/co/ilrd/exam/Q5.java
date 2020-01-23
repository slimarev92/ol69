package il.co.ilrd.exam;

public class Q5 {

	public static void printMissingNums(int[] arr, int low, int high) {
		int[] presentNumbers = new int[high - low + 1];
		
		for(int i = 0; i < arr.length; ++i) {
			presentNumbers[arr[i] - low] = 1;
		}
		
		for(int i = 0; i < presentNumbers.length; ++i) {
			if(1 != presentNumbers[i]) {
				System.out.println(i + low);
			}
		}
	}
	
	public static void main(String[] args) {
		printMissingNums(new int[] {10,  12,  11, 15}, 10, 15);
	}
	
}
