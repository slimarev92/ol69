package il.co.ilrd.tests;

import java.util.ConcurrentModificationException;
import java.util.Iterator;
import il.co.ilrd.genericlinkedlist.GenericLinkedList;
import org.junit.Assert;
import org.junit.Test;

public class GenericSinglyLinkedListTest {
	
	@Test
	public void testPushPop() {
		GenericLinkedList<Integer> list = new GenericLinkedList<>();
		
		list.pushFront(10);
		list.pushFront(20);
		list.pushFront(30);
		
		int actual = list.popFront();
		
		Assert.assertEquals(30, actual);
	}
	
	@Test
	public void testSize() {
		GenericLinkedList<Integer> list = new GenericLinkedList<>();
		
		list.pushFront(10);
		list.pushFront(20);
		list.pushFront(30);
		list.pushFront(40);
		list.pushFront(50);
		list.pushFront(60);
		list.pushFront(70);
		list.pushFront(80);
		
		list.popFront();
		
		int expectedSize = 7;
		int actualSize = list.size(); 
		
		Assert.assertEquals(expectedSize, actualSize);
	}
	
	@Test
	public void testIsEmpty() {
		GenericLinkedList<Integer> list = new GenericLinkedList<>();
		
		boolean expectedIsEmpty = true;
		boolean actualIsEmpty = list.isEmpty();
		
		Assert.assertEquals(expectedIsEmpty, actualIsEmpty);
		
		list.pushFront(20);
		
		expectedIsEmpty = false;
		actualIsEmpty = list.isEmpty();
		
		Assert.assertEquals(expectedIsEmpty, actualIsEmpty);
	}
	
	@Test
	public void testFind() {
		GenericLinkedList<Integer> list = new GenericLinkedList<>();
		
		list.pushFront(10);
		list.pushFront(20);
		list.pushFront(30);
		list.pushFront(40);
		list.pushFront(50);
		list.pushFront(60);
		list.pushFront(70);
		list.pushFront(80);
		
		int expectedValue = 10;
		int actualValue = list.find(10).next();
		
		Assert.assertEquals(expectedValue, actualValue);
		
		Iterator<Integer> found = list.find(100);
		
		Assert.assertEquals(false, found.hasNext());
	}
	
	@Test
	public void testReverse() {
		GenericLinkedList<Integer> list = new GenericLinkedList<>();
		
		list.pushFront(10);
		list.pushFront(20);
		list.pushFront(30);
		list.pushFront(40);
		list.pushFront(50);
		list.pushFront(60);
		list.pushFront(70);
		list.pushFront(80);
		
		GenericLinkedList<Integer> listReverse = GenericLinkedList.newReverse(list);
		
		int expectedSize = 8;
		int actualSize = listReverse.size();
		
		Assert.assertEquals(expectedSize, actualSize);
		
		int[] expectedVals = {10, 20, 30, 40, 50, 60, 70, 80};
		
		for(int i = 0; i < expectedVals.length; ++i) {
			int actualVal = listReverse.popFront();
			
			Assert.assertEquals(expectedVals[i], actualVal);
		}
	}
	
	@Test
	public void testMerge() {
		GenericLinkedList<Integer> l1 = new GenericLinkedList<>();
		GenericLinkedList<Integer> l2 = new GenericLinkedList<>();
		
		l1.pushFront(4);
		l1.pushFront(3);
		l1.pushFront(2);
		l1.pushFront(1);
		
		l2.pushFront(7);
		l2.pushFront(6);
		l2.pushFront(5);
		
		GenericLinkedList<Integer> merged = GenericLinkedList.newMerge(l1, l2);
		int[] expectedVals = {1, 2, 3, 4, 5, 6, 7};		
		
		for(int i = 0; i < expectedVals.length; ++i) {
			int actualVal = merged.popFront();
			
			Assert.assertEquals(expectedVals[i], actualVal);
		}		
	}

	@Test
	public void testGeneric() {
		GenericLinkedList<String> list = new GenericLinkedList<>();
		
		list.pushFront("Hello");
		list.pushFront("my");
		list.pushFront("name");
		list.pushFront("is");
		list.pushFront("Jimmy");
		list.pushFront("Pop");
		
		int expectedSize = 6;
		int actualSize = list.size();
		
		Assert.assertEquals(expectedSize, actualSize);
	}
	
	@Test(expected = ConcurrentModificationException.class)
	public void exceptionTest() {
		GenericLinkedList<String> list = new GenericLinkedList<>();
		
		list.pushFront("Hello");
		list.pushFront("my");
		list.pushFront("name");
		list.pushFront("is");
		list.pushFront("Jimmy");
		list.pushFront("Pop");
		
		Iterator<String> iter = list.iterator();
		list.pushFront("and im a dumb white guy");
		
		iter.next();
	}
}
