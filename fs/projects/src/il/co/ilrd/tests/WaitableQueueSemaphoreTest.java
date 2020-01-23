package il.co.ilrd.tests;

import org.junit.Test;

import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.atomic.AtomicInteger;
import org.junit.Assert;
import il.co.ilrd.concurrency.WaitableQueue;
import il.co.ilrd.concurrency.WaitableQueueSemaphore;


public class WaitableQueueSemaphoreTest {

	@Test
	public void testSimple() throws InterruptedException {
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>();
		
		queue.enqueue(50);
		queue.enqueue(30);
		queue.enqueue(10);
		queue.enqueue(20);
		queue.enqueue(40);
		
		queue.dequeue(); /* remove 10 */
		
		Boolean expectedResult = false;
		Boolean actualResult = queue.remove(10);
		
		Assert.assertEquals(expectedResult, actualResult);
		
		Integer expectedNum = 20;
		Integer actualNum = queue.dequeue();
		
		Assert.assertEquals(expectedNum, actualNum);
	}
	
	@Test
	public void testSimpleComparator()  throws InterruptedException {
		/* sorts numbers in reverse order */
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>((num1, num2) -> num2 - num1);
		
		queue.enqueue(50);
		queue.enqueue(30);
		queue.enqueue(10);
		queue.enqueue(20);
		queue.enqueue(40);
		
		queue.dequeue(); /* remove 50 */
		
		Integer expectedNum = 40;
		Integer actualNum = queue.dequeue();
		
		Assert.assertEquals(expectedNum, actualNum);
	}
	
	@Test
	public void testProducerConsumer() throws InterruptedException {
		int numConsumersAndProducers = 10;
		final int itersPerThread = 100000;
		AtomicInteger totalItersProducers = new AtomicInteger(0);
		AtomicInteger totalItersConsumers = new AtomicInteger(0);
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>();
		Semaphore finishedProducers = new Semaphore(0);
		Semaphore finishedConsumers = new Semaphore(0);
		
		for(int i = 0; i < numConsumersAndProducers; ++i) {
			new Producer(queue, itersPerThread, totalItersProducers, finishedProducers).start();
			new Consumer(queue, itersPerThread, totalItersConsumers, finishedConsumers).start();
		}
		try {
			finishedProducers.acquire(numConsumersAndProducers);
			finishedConsumers.acquire(numConsumersAndProducers);
			
		} catch(InterruptedException e) { e.printStackTrace(); }
		
		
		int expectedVal = numConsumersAndProducers * itersPerThread;
		int actualVal = totalItersProducers.get();
		
		Assert.assertEquals(expectedVal, actualVal);
		
		expectedVal = numConsumersAndProducers * itersPerThread;
		actualVal = totalItersConsumers.get();
		
		Assert.assertEquals(expectedVal, actualVal);
	}
	
	@Test(expected = TimeoutException.class)
	public void testTimeOutFail() throws TimeoutException, InterruptedException {
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>();

		queue.dequeue(1500 , TimeUnit.MILLISECONDS);
	}
	
	@Test
	public void testTimeOutSuccess() throws TimeoutException {
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>();
		Thread.interrupted(); /* due to bug in junit 4 */
		
		Thread insertAfterPause = new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					Thread.sleep(3000);
				} catch (InterruptedException e) {}
				queue.enqueue(10);
			}
		});
		
		insertAfterPause.start();
		Integer expectedValue = 10;
		Integer actualValue = 0;
		
		try {
			actualValue = queue.dequeue(100, TimeUnit.SECONDS);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		Assert.assertEquals(expectedValue, actualValue);
	}

	@Test(expected = InterruptedException.class)
	public void testTimeOutWithInterrupt() throws TimeoutException, InterruptedException {
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>();
		Thread currThread = Thread.currentThread();
		
		Thread insertAfterPause = new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					Thread.sleep(1000);
					currThread.interrupt();
				} catch (InterruptedException e) {}
			}
		});
		
		insertAfterPause.start();
		queue.dequeue(100000, TimeUnit.DAYS);
	}
	
	@Test
	public void testBlockingDequeue() throws InterruptedException {
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>();

		Thread insertAfterPause = new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {}
				queue.enqueue(10);
			}
		});
		
		insertAfterPause.start();
		Integer expectedValue = 10;
		Integer actualValue = queue.dequeue();
		
		Assert.assertEquals(expectedValue, actualValue);
	}
	
	@Test(expected = InterruptedException.class)
	public void testBlockingDequeueWithInterrupt() throws InterruptedException {
		WaitableQueue<Integer> queue = new WaitableQueueSemaphore<>();
		Thread currThread = Thread.currentThread();
		
		Thread insertAfterPause = new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					Thread.sleep(1500);
				} catch (InterruptedException e) {}
				currThread.interrupt();
			}
		});
		
		insertAfterPause.start();
		Integer expectedValue = 10;
		Integer actualValue = queue.dequeue();
		
		Assert.assertEquals(expectedValue, actualValue);
	}
	
	private static class Producer extends Thread {
		private final WaitableQueue<Integer> queue;
		private final int numIters;
		private final AtomicInteger totalIters;
		private final Semaphore finished;
		
		public Producer(WaitableQueue<Integer> queue, int numIters, AtomicInteger totalIters, Semaphore finished) {
			this.queue = queue;
			this.numIters = numIters;
			this.totalIters = totalIters;
			this.finished = finished;
		}
			
		@Override
		public void run() {
			for(int i = 0; i < numIters; ++i) {
				queue.enqueue(i);
				totalIters.incrementAndGet();
			}
			
			finished.release();
		}
	}
	
	private static class Consumer extends Thread {
		private final WaitableQueue<Integer> queue;
		private final int numIters;
		private final AtomicInteger totalIters;
		private final Semaphore finished;
		
		public Consumer(WaitableQueue<Integer> queue, int numIters, AtomicInteger totalIters, Semaphore finished) {
			this.queue = queue;
			this.numIters = numIters;
			this.totalIters = totalIters;
			this.finished = finished;
		}
		
		@Override
		public void run() {
			for(int i = 0; i < numIters; ++i) {
				Integer curr = null;
				try {
					curr = queue.dequeue();
				} catch (InterruptedException e) { e.printStackTrace(); }
				if(null != curr) {
					totalIters.incrementAndGet();					
				}
			}
			
			finished.release();
		}
	}
}
