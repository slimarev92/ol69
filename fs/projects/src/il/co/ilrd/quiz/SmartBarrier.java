package il.co.ilrd.quiz;

import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class SmartBarrier {
	private final Semaphore numCanEnter = new Semaphore(0);
	private final Semaphore numCanExit = new Semaphore(0);
	private final int maxCount;
	private int wentIn = 0;
	private int wentOut = 0;
	private final Lock gateLock = new ReentrantLock();
	private static AtomicInteger totalWentThrough = new AtomicInteger(0);
	
	public SmartBarrier(int maxCount) {
		this.maxCount = maxCount;
	}
	
	public void lock() throws InterruptedException {
		gateLock.lock();
		
		if ((totalWentThrough.get() % maxCount) != 0) { /* for testing purposes */
			System.out.println("bad!");
		} else {
			System.out.println(totalWentThrough.get());
		}

		try {
			if(wentIn == (maxCount - 1)) {
				numCanEnter.release(maxCount);
			} else {
				++wentIn;
			}
		} finally { gateLock.unlock(); }
		
		numCanEnter.acquire();
		
		gateLock.lock();
		
		try {
			if(wentOut == (maxCount - 1)) {
				wentIn = 0;
				wentOut = 0;
				numCanExit.release(maxCount);
			} else {
				++wentOut;
			}
		
		} finally { totalWentThrough.incrementAndGet(); gateLock.unlock(); }
		
		numCanExit.acquire();
	}
	
	private static class ButtonPusher extends Thread {
		private final SmartBarrier barrier;
		
		public ButtonPusher(SmartBarrier barrier) {
			this.barrier = barrier;
		}
		
		@Override
		public void run() {
			while(true) {
				try {
					barrier.lock();
				} catch(InterruptedException e) { }	
			}
		}
	}	
	
	public static void main(String[] args) throws InterruptedException {
		int numOfPushers = 1000;
		SmartBarrier barrier = new SmartBarrier(numOfPushers);
		
		for(int i = 0; i < 1000; ++i) {
			new ButtonPusher(barrier).start();
		}
		
	}
}
