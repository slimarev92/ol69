package il.co.ilrd.tests;

import org.junit.Test;
import org.junit.Assert;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.Map;
import java.util.LinkedHashMap;

import il.co.ilrd.hashmap.SashMap;

public class HashMapTest {
	
	@Test
	public void testIsEmpty() {
		SashMap<String, String> map = new SashMap<>();
		
		boolean expectedResult = true;
		boolean actualResult = map.isEmpty();
		
		Assert.assertEquals(expectedResult, actualResult);
		
		map.put("key", "value");
		
		expectedResult = false;
		actualResult = map.isEmpty();
		
		Assert.assertEquals(expectedResult, actualResult);	
	}
	
	@Test
	public void testPutRemoveGetSize() {
		SashMap<String, Integer> map = new SashMap<>(64);
		
		map.put("a", 1);  
		map.put("b", 2);  
		map.put("c", 3);  
		map.put("d", 4);  
		map.put("e", 5); 
		map.put("e", 6);
		map.put("f", 7);
		
		map.remove("c");
		map.remove("cccccccc");
		
		int expectedSize = 5;
		int actualSize = map.size();
		
		Assert.assertEquals(expectedSize, actualSize);
		
		Integer expectedValue = 4;
		Integer actualValue = map.get("d");
		
		Assert.assertEquals(expectedValue, actualValue); 
		
		expectedValue = null;
		actualValue = map.get("nehama");
		
		Assert.assertEquals(expectedValue, actualValue);
	}
	
	@Test
	public void testPutAll() {
		Map<Integer, Integer> mapInput = new LinkedHashMap<>();
		Map<Integer, Integer> map = new SashMap<>();
		
		mapInput.put(10, 10);
		mapInput.put(20, 10);
		mapInput.put(30, 10);
		mapInput.put(40, 10);
		
		map.putAll(mapInput);
		
		int expectedSize = 4;
		int actualSize = map.size();
		
		Assert.assertEquals(expectedSize, actualSize);
	}
	
	@Test
	public void testClear() {
		SashMap<String, Integer> map = new SashMap<>();
		
		map.put("nehama", 1);
		map.put("dvora", 2);
		map.put("billy", 3);
		map.put("olive", 4);
		map.put("becky", 5);
		map.put("georgia", 6);
		
		map.clear();
		
		int expectedSize = 0;
		int actualSize = map.size();
		
		Assert.assertEquals(expectedSize, actualSize);
	}
	
	@Test
	public void testContainsKeyValue() {
		SashMap<String, Integer> map = new SashMap<>();
		
		map.put("nehama", 1);
		map.put("dvora", 2);
		map.put("billy", 3);
		map.put("olive", 4);
		map.put("becky", 5);
		map.put("georgia", 6);
		
		boolean expectedResult = true;
		boolean actualResult = map.containsKey("olive");
		
		Assert.assertEquals(expectedResult, actualResult);
		
		expectedResult = false;
		actualResult = map.containsKey("oliveeee");
		
		Assert.assertEquals(expectedResult, actualResult);
		
		
		expectedResult = true;
		actualResult = map.containsValue(5);
		
		Assert.assertEquals(expectedResult, actualResult);
		
		expectedResult = false;
		actualResult = map.containsValue(23);
		
		Assert.assertEquals(expectedResult, actualResult);	
	}
	
	@Test
	public void testEntrySet() {
		Map<Integer, Integer> map = new SashMap<>(30);
		
		map.put(1, 1);
		map.put(2, 2);
		map.put(3, 3);
		map.put(4, 4);
		map.put(5, 5);
		map.put(6, 6);
		map.put(7, 7);
		map.put(8, 8);
		map.put(9, 9);
		map.put(10, 10);
		
		Iterator<Map.Entry<Integer, Integer>> iter = map.entrySet().iterator();
		Integer[] expectedVals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		
		for(Integer expectedValue : expectedVals) {
			Integer actualValue = iter.next().getValue();
			
			Assert.assertEquals(expectedValue, actualValue);
		}	
	}
	
	@Test
	public void testKeySet() {
		Map<Integer, Integer> map = new SashMap<>(30);
		
		map.put(1, 1);
		map.put(2, 2);
		map.put(3, 3);
		map.put(4, 4);
		map.put(5, 5);
		map.put(6, 6);
		map.put(7, 7);
		map.put(8, 8);
		map.put(9, 9);
		map.put(10, 10);
		
		Iterator<Integer> iter = map.keySet().iterator();
		Integer[] expectedVals = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		
		for(Integer expectedValue : expectedVals) {
			Integer actualValue = iter.next();
			
			Assert.assertEquals(expectedValue, actualValue);
		}	
		
		Integer expectedSize = 10;
		Integer actualSize = map.keySet().size();
		
		Assert.assertEquals(expectedSize, actualSize);
	}
	
	@Test
	public void testNullKeyVal() {
		Map<String, Integer> map = new SashMap<>();
		
		map.put("nehama", 1);
		map.put("dvora", 2);
		map.put("billy", 3);
		map.put("olive", 4);
		map.put("becky", 5);
		map.put("georgia", 6);
		map.put(null, 7);
		map.put("Nully", null);
		
		boolean expectedResult = true;
		boolean actualResult = map.containsKey(null);
		
		Assert.assertEquals(expectedResult, actualResult);
		
		expectedResult = true;
		actualResult = map.containsValue(null);
		
		Assert.assertEquals(expectedResult, actualResult);
		
		int expectedSize = 8;
		int actualSize = map.size();
		
		Assert.assertEquals(expectedSize, actualSize);
	}
	
	@Test(expected = ConcurrentModificationException.class)
	public void testException1() {
		Map<Integer, Integer> map = new SashMap<>(30);
		
		map.put(1, 1);
		
		Iterator<Integer> iter = map.keySet().iterator();
		
		map.put(2, 2);

		iter.hasNext();
	}
}
