package il.co.ilrd.exam;

import java.util.Arrays;

public class Q1A {

	public static void swapIndices(int[] arr, int from, int to) {
		int temp = arr[from];
		arr[from] = arr[to];
		arr[to] = temp;
	}
	
	public static void reverseSubarray(int[] arr, int from, int to) {
		for(; from < to; ++from, --to) {
			 swapIndices(arr,from, to);
		}
	}
	
	public static void shift(int[] arr, int n) {
		n %= arr.length;
		
		reverseSubarray(arr, 0, arr.length - 1 - n);
		reverseSubarray(arr, 0, arr.length - 1);
		reverseSubarray(arr, 0, n - 1);
	}

	public static void main(String[] args) {
		int[] arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		
		System.out.println(Arrays.toString(arr));
		
		shift(arr, 7);
		
		System.out.println(Arrays.toString(arr));
 	}
}
