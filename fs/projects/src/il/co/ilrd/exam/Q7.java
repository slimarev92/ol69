package il.co.ilrd.exam;

public class Q7 {
	
	public static void printPermutations(String word) {	
		printPermutationsRec(word, "", new boolean[word.length()]);
	}
	
	private static void printPermutationsRec(String word, String curr, boolean[] usedChars) {
		if(outOfOptions(usedChars)) {
			System.out.println(curr);
		}
		
		for(int i = 0; i < usedChars.length; ++i) {
			if(false == usedChars[i]) {
				usedChars[i] = true;
				
				printPermutationsRec(word, curr + word.charAt(i), usedChars);
				
				usedChars[i] = false;
			}
		}
	}
	
	private static boolean outOfOptions(boolean[] usedChars) {
		for(boolean curr : usedChars) {
			if(false == curr) {
				return false;
			}
		}
		
		return true;
	}

	public static String reverseString(String str) {
		char[] strArr = str.toCharArray();
		
		reverseStringRec(strArr, 0, strArr.length - 1);
		
		return new String(strArr);
	}
	
	private static void reverseStringRec(char[] strArr, int left, int right) {
		if(left >= right) {
			return;
		} else {
			char temp = strArr[left];
			strArr[left] = strArr[right];
			strArr[right] = temp;
			
			reverseStringRec(strArr, left + 1, right - 1); 
		}
	}
	
	public static void main(String[] args) {
		//PART A
		String original = "1234567890";
		
		System.out.println(reverseString(original));
		
		//PART B
		printPermutations("123");
	}
}
