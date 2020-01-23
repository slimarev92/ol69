package il.co.ilrd.quiz;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Barrier {
	private final int initialValue;
	private final ReentrantLock lock = new ReentrantLock(true);
	private final Condition condition = lock.newCondition();
	private volatile CountDownLatch latch;
	private AtomicInteger counter = new AtomicInteger(0);
	private volatile int successCounter = 0;
	
	public Barrier(int initialValue) {
		this.initialValue = initialValue;
		latch = new CountDownLatch(initialValue);
	}
	
	public void lock() {
		boolean doneWaiting = false;
	
		lock.lock();
		
		try {
			counter.set(0);
 		} finally {
 			lock.unlock();
 		}
		
		latch.countDown();
		
		while(!doneWaiting) {
			try {
				latch.await();
			} catch (InterruptedException e) { continue; }
			 doneWaiting = true;
		}
		
		lock.lock();
		
		try {
			if (counter.get() == initialValue - 1) {
				latch = new CountDownLatch(initialValue);
				counter.incrementAndGet();
				System.out.println("done " + successCounter++);
				condition.signalAll();
			} else {
				counter.incrementAndGet();

				while(counter.get() < initialValue) {
					try {
						condition.await();
					} catch (InterruptedException e) { continue; }
				}
			}
		} finally {
			lock.unlock();
		}
	}
	
	private static class ButtonPusher extends Thread {
		private final Barrier barrier;
		
		public ButtonPusher(Barrier barrier) {
			this.barrier = barrier;
		}
		
		@Override
		public void run() {
			while(true) {
				barrier.lock();
			}
		}
	}
	
	public static void main(String[] args) {
		int numOfPushers = 1000;
		Barrier barrier = new Barrier(numOfPushers);
		
		for(int i = 0; i < numOfPushers; ++i) {
			new ButtonPusher(barrier).start();
		}
		
	}
}