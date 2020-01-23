package il.co.ilrd.tests;

import java.util.Stack;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import il.co.ilrd.concurrency.ThreadPool;
import il.co.ilrd.concurrency.ThreadPool.Priority;

public class ThreadPoolTest {

	@Test
	public void testDefault() throws InterruptedException {
		for(int numTests = 0; numTests < 3; ++numTests) {
			ThreadPool pool = new ThreadPool();
			AtomicInteger number = new AtomicInteger(0);
			int numOfTasks = 1000000;
			CountDownLatch allDone = new CountDownLatch(1);
			
			for(int j = 0; j < numOfTasks; ++j) {
				pool.submit(() -> number.incrementAndGet(), Priority.MEDIUM);
			}
			
			pool.submit(() -> allDone.countDown(), Priority.LOW);
			
			try {
				allDone.await();
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			pool.shutdown();
			
			Integer expectedValue = numOfTasks;
			Integer actualValue = number.intValue();
			
			Assert.assertEquals(expectedValue, actualValue);
			
			boolean expectedAllDone = true;
			boolean actualAllDone = pool.awaitTermination(100, TimeUnit.MILLISECONDS);
					
			Assert.assertEquals(expectedAllDone, actualAllDone);
		}	
	}
	
	@Test
	public void testNumOfThreadsChange() throws InterruptedException {
		for(int numTests = 0; numTests < 10; ++numTests) {
			ThreadPool pool = new ThreadPool(10);
			AtomicInteger number = new AtomicInteger(0);
			int numOfTasks = 1000000;
			CountDownLatch allDone = new CountDownLatch(1);
			
			for(int j = 0; j < numOfTasks / 2; ++j) {
				pool.execute(() -> number.incrementAndGet());
			}
			
			pool.setNumThreads(1);
			pool.setNumThreads(20);
			pool.setNumThreads(1);
			pool.setNumThreads(20);
			
			for(int j = 0; j < numOfTasks / 2; ++j) {
				pool.submit(() -> number.incrementAndGet(), Priority.MEDIUM);
			}
			
			
			pool.submit(() -> allDone.countDown(), Priority.LOW);
			
			try {
				allDone.await();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
			pool.shutdown();
			
			Integer expectedValue = numOfTasks;
			Integer actualValue = number.intValue();
			
			Assert.assertEquals(expectedValue, actualValue);
			
			boolean expectedAllDone = true;
			boolean actualAllDone = pool.awaitTermination(100, TimeUnit.MILLISECONDS);

			Assert.assertEquals(expectedAllDone, actualAllDone);
		}
	}	
	
	@Test
	public void testAwaitTermination() throws InterruptedException {
		for(int numTests = 0; numTests < 5; ++numTests) {
			ThreadPool pool = new ThreadPool(1);

			pool.submit(() -> {
				Thread.interrupted();
				TimeUnit.HOURS.sleep(24);
				return 1;
			});
			
			pool.shutdown();
			
			try {
				Thread.sleep(0, 100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

			boolean expected = false;
			boolean actual = pool.awaitTermination(100, TimeUnit.MILLISECONDS);
		
			Assert.assertEquals(expected, actual);
		}	
	}
	
	@Test
	public void testExceptions() {
		for(int numTests = 0; numTests < 10; ++numTests) {

			Exception actualException = null;
			
			try {
				new ThreadPool(-1);
			} catch(Exception e) {
				actualException = e;
			}
			
			Assert.assertEquals(IllegalArgumentException.class, actualException.getClass());
			
			try {
				ThreadPool pool = new ThreadPool();
				pool.shutdown();
				pool.submit(() -> 0);
				
			} catch(Exception e) {
				actualException = e;
			}
			
			try {
				ThreadPool pool = new ThreadPool();
				pool.shutdown();
				pool.setNumThreads(100);
				
			} catch(Exception e) {
				actualException = e;
			}
			
			String expectedMessage = "Thread pool was shut down";
			String actualMessage = actualException.getMessage();
			
			Assert.assertEquals(expectedMessage, actualMessage);
			
			try {
				ThreadPool pool = new ThreadPool();
				pool.resume();
				
			} catch(Exception e) {
				actualException = e;
			}
			
			expectedMessage = "Can't resume unpaused pool";
			actualMessage = actualException.getMessage();
			
			Assert.assertEquals(expectedMessage, actualMessage);
			
			try {
				ThreadPool pool = new ThreadPool();
				pool.pause();
				pool.pause();
				
			} catch(Exception e) {
				actualException = e;
			}
			
			expectedMessage = "Can't pause twice";
			actualMessage = actualException.getMessage();
			
			Assert.assertEquals(expectedMessage, actualMessage);
			
			try {
				ThreadPool pool = new ThreadPool();
				pool.shutdown();
				pool.shutdown();
				
			} catch(Exception e) {
				actualException = e;
			}
			
			expectedMessage = "Can't shutdown twice";
			actualMessage = actualException.getMessage();
			
			Assert.assertEquals(expectedMessage, actualMessage);
			
			try {
				ThreadPool pool = new ThreadPool();
				pool.awaitTermination(10, TimeUnit.DAYS);
			} catch(Exception e) {
				actualException = e;
			}
			
			expectedMessage = "Thread Pool wasn't shut down";
			actualMessage = actualException.getMessage();

			Assert.assertEquals(expectedMessage, actualMessage);
			
			try {
				ThreadPool pool = new ThreadPool();
				pool.pause();
				Future<?> future = pool.submit(() -> 0);
				future.cancel(true);
				future.get();
				pool.resume();
				
			} catch(Exception e) {
				actualException = e;
			}
			
			expectedMessage = "Task was cancelled";
			actualMessage = actualException.getMessage();
			
			Assert.assertEquals(expectedMessage, actualMessage);
			
			try {
				ThreadPool pool = new ThreadPool();
		
				Future<?> future = pool.submit(() -> {
					Thread.sleep(10000);
					return 1;
				});
				
				future.get(1, TimeUnit.NANOSECONDS);
				
			} catch(Exception e) {
				actualException = e;
			}
			
			expectedMessage = "Task wasn't completed in time";
			actualMessage = actualException.getMessage();
			
			Assert.assertEquals(expectedMessage, actualMessage);
			
			try {
				ThreadPool pool = new ThreadPool();
		
				Future<?> future = pool.submit(() -> {return 1 / 0;});
				
				future.get();
				
			} catch(Exception e) {
				actualException = e;
			}
					
			Class<?> expectedType = ArithmeticException.class;
			Class<?> actualType =  actualException.getCause().getClass();
			
			Assert.assertEquals(expectedType, actualType);
			
			try {
				ThreadPool pool = new ThreadPool();
		
				pool.setNumThreads(-1);
				
			} catch(Exception e) {
				actualException = e;
			}
					
			expectedMessage = "Number of threads must be non-negative";
			actualMessage = actualException.getMessage();
			
			Assert.assertEquals(expectedMessage, actualMessage);
		}
	}
	
	@Test
	public void testPauseResumeSimple() throws InterruptedException {
		for(int numTests = 0; numTests < 5; ++numTests) {
			ThreadPool pool = new ThreadPool();
			AtomicInteger number = new AtomicInteger(0);
			int numOfTasks = 1000000;
			CountDownLatch allDone = new CountDownLatch(1);

			pool.pause();
			
			pool.submit(() -> allDone.countDown(), Priority.LOW);
		
			for(int i = 0; i < numOfTasks; ++i) {
				pool.submit(() -> number.addAndGet(1), Priority.HIGH);
			}
					
			pool.resume();
			
			try {
				allDone.await();
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			pool.shutdown();
			
			Integer expectedValue = numOfTasks;
			Integer actualValue = number.intValue();
			
			Assert.assertEquals(expectedValue, actualValue);
			
			boolean expectedAllDone = true;
			boolean actualAllDone = pool.awaitTermination(100, TimeUnit.MILLISECONDS);
					
			Assert.assertEquals(expectedAllDone, actualAllDone);
		}
	}
	
	@Test
	public void testPriority() throws InterruptedException, ExecutionException {
		for(int numTests = 0; numTests < 10; ++numTests) {
			ThreadPool pool = new ThreadPool(1);
			Stack<Integer> results = new Stack<>();
			
			pool.pause();
		
			Future<Integer> future1 = pool.submit(() -> results.push(1), Priority.LOW, new Integer(1));
			Future<Integer> future2 = pool.submit(() -> results.push(2), Priority.MEDIUM, new Integer(2));
			Future<Integer> future3 = pool.submit(() -> results.push(3), Priority.HIGH,  new Integer(3));

			pool.resume();
					
			try {
				Thread.sleep(120);
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			Integer[] expectedVals = {1, 2, 3};
			
			for(Integer expectedValue : expectedVals) {
				Assert.assertEquals(expectedValue, results.pop());
			}
			
			Assert.assertEquals(new Integer(1), future1.get());
			Assert.assertEquals(new Integer(2), future2.get());
			Assert.assertEquals(new Integer(3), future3.get());
		}
	}
	
	@Test
	public void testShutdown() throws InterruptedException {
		for(int j = 0; j < 10; ++j) {
			ThreadPool pool = new ThreadPool(1);
			Stack<Integer> results = new Stack<>();
			
			pool.pause();
		
			pool.submit(() ->  results.push(1), Priority.LOW);
			pool.submit(() ->  results.push(2), Priority.MEDIUM);
			pool.submit(() ->  results.push(3), Priority.HIGH);
			
			pool.resume();
			
			pool.shutdown();
		
			Boolean expectedValue = true;
			Boolean actualValue = pool.awaitTermination(100, TimeUnit.MILLISECONDS);

			Assert.assertEquals(expectedValue, actualValue);
		}	
	}
	
	@Test
	public void testCancelWhileWaiting() {
		for(int j = 0; j < 10; ++j) {
			ThreadPool pool = new ThreadPool(1);
			Stack<Integer> results = new Stack<>();
			
			pool.pause();
		
			Future<?> future1 = pool.submit(() ->  results.push(1), Priority.LOW);
			Future<?> future2 = pool.submit(() ->  results.push(2), Priority.MEDIUM);
			Future<?> future3 = pool.submit(() ->  results.push(3), Priority.HIGH);
			
			Assert.assertFalse(future1.isDone());
			
			future1.cancel(true);
			future2.cancel(true);
			future3.cancel(true);
			
			pool.resume();
					
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			Boolean expectedValue = true;
			Boolean actualValue = results.isEmpty();

			Assert.assertEquals(expectedValue, actualValue);
			
			Assert.assertTrue(future1.isDone());
			Assert.assertTrue(future2.isDone());
			Assert.assertTrue(future3.isDone());
			
			expectedValue = true;
			actualValue = future1.isCancelled() && 
					      future2.isCancelled() && 
					      future3.isCancelled();
			
			Assert.assertEquals(expectedValue, actualValue);
		}
	}
}
