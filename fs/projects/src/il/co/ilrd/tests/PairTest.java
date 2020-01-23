package il.co.ilrd.tests;

import org.junit.Assert;
import org.junit.Test; 
import il.co.ilrd.pair.Pair;

public class PairTest {

	@Test
	public void testGet() {
		Pair<String, Integer> pair = Pair.of("Eldad", 10);
		
		String expectedKey ="Eldad";
		Integer expectedValue = 10;
		
		Assert.assertEquals(expectedKey, pair.getKey());
		Assert.assertEquals(expectedValue, pair.getValue());
	}
	
	@Test
	public void testSet() {
		Pair<String, Integer> pair = Pair.of("Eldad", 10);
		
		pair.setKey("Sasha");
		pair.setValue(20);
		
		String expectedKey ="Sasha";
		Integer expectedValue = 20;
		
		Assert.assertEquals(expectedKey, pair.getKey());
		Assert.assertEquals(expectedValue, pair.getValue());
	}
	
	@Test
	public void testToString() {
		Pair<String, Integer> pair = Pair.of("Eldad", 10);
		
		String expectedStr = "Key: Eldad, Value: 10"; 
		
		Assert.assertEquals(expectedStr, pair.toString());
	}
	
	@Test
	public void testEquals() {
		Pair<String, Integer> pair = Pair.of("Eldad", 10);
		Pair<String, Integer> pair2 = Pair.of("Eldad", 10);
		
		boolean expectedResult = true;
		
		Assert.assertEquals(expectedResult, pair.equals(pair2));
		
		pair2.setKey("Eldaddddd");
		expectedResult = false;
		
		Assert.assertEquals(expectedResult, pair.equals(pair2));
	}
	
	@Test
	public void testHashCode() {
		Pair<String, Integer> pair = Pair.of("Eldad", 10);
		Pair<String, Integer> pair2 = Pair.of("Eldad", 10);
				
		Assert.assertEquals(pair.hashCode(), pair2.hashCode());
	}
	
	@Test
	public void testMinMax1() {
		Integer[] arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		Pair<Integer, Integer> result = Pair.minMax(arr);
		
		Integer expectedKey = 1;
		Integer expectedValue = 10;
		
		Assert.assertEquals(expectedKey, result.getKey());
		Assert.assertEquals(expectedValue, result.getValue());
	}
	
	@Test
	public void testMinMax2() {
		Integer[] arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		Pair<Integer, Integer> result = Pair.minMax(arr, (Integer first, Integer second) -> (-1 * first.compareTo(second)));
		
		Integer expectedKey = 10;
		Integer expectedValue = 1; 
		
		Assert.assertEquals(expectedKey, result.getKey());
		Assert.assertEquals(expectedValue, result.getValue());
	}
	
	
}
