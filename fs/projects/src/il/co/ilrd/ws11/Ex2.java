package il.co.ilrd.ws11;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReentrantLock;

import il.co.ilrd.stopwatch.StopWatch;

public class Ex2 {
		
	private static class NoSync {
		private static int counter = 0;
		private static final int MAX_ITERS = 10000000;
		
		private static class Incrementer extends Thread {
			@Override
			public void run() {
				for(int i = 0; MAX_ITERS > i; ++i) {
					++counter;
				}
			}
		}
		
		public static void runTwoIncrementers() {
			Incrementer one = new Incrementer();
			Incrementer two = new Incrementer();
			StopWatch stopWatch = new StopWatch();
			
			stopWatch.start();
			one.start();
			two.start();
			
			try {
				one.join();
				two.join();
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			stopWatch.stop();
			
			System.out.println("Final value of counter: " + counter);
			System.out.println("Total time elapsed: " +
			                    stopWatch.timeElapsedMillis() +
			                    " milliseconds");
		}
	}

	private static class SynchronizedMethod {
		private static int counter = 0;
		private static final int MAX_ITERS = 10000000;
		
		private static class Incrementer extends Thread {
			@Override
			public void run() {
				for(int i = 0; MAX_ITERS > i; ++i) {
					SynchronizedMethod.incrementCounter();
				}
			}
		}
		
		private static synchronized void incrementCounter() {
			++counter;
		}
		
		public static void runTwoIncrementers() {
			Incrementer one = new Incrementer();
			Incrementer two = new Incrementer();
			StopWatch stopWatch = new StopWatch();
			
			stopWatch.start();
			one.start();
			two.start();
			
			try {
				one.join();
				two.join();
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			stopWatch.stop();
			
			System.out.println("Final value of counter: " + counter);
			System.out.println("Total time elapsed: " +
			                    stopWatch.timeElapsedMillis() +
			                    " milliseconds");
		}
	}

	private static class Atomic {
		private static AtomicInteger counter = new AtomicInteger();
		private static final int MAX_ITERS = 10000000;
		
		private static class Incrementer extends Thread {
			@Override
			public void run() {
				for(int i = 0; MAX_ITERS > i; ++i) {
					counter.incrementAndGet();
				}
			}
		}
		
		public static void runTwoIncrementers() {
			Incrementer one = new Incrementer();
			Incrementer two = new Incrementer();
			StopWatch stopWatch = new StopWatch();
			
			stopWatch.start();
			one.start();
			two.start();
			
			try {
				one.join();
				two.join();
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			stopWatch.stop();
			
			System.out.println("Final value of counter: " + counter.intValue());
			System.out.println("Total time elapsed: " +
			                    stopWatch.timeElapsedMillis() +
			                    " milliseconds");
		}
	}

	private static class ReentLock {
		private static int counter;
		private static final int MAX_ITERS = 10000000;
		private static ReentrantLock counterLock = new ReentrantLock();
		
		private static class Incrementer extends Thread {
			@Override
			public void run() {
				for(int i = 0; MAX_ITERS > i; ++i) {
					counterLock.lock();
					++counter;
					counterLock.unlock();
				}
			}
		}
		
		public static void runTwoIncrementers() {
			Incrementer one = new Incrementer();
			Incrementer two = new Incrementer();
			StopWatch stopWatch = new StopWatch();
			
			stopWatch.start();
			one.start();
			two.start();
			
			try {
				one.join();
				two.join();
			} catch (InterruptedException e) { e.printStackTrace(); }
			
			stopWatch.stop();
			
			System.out.println("Final value of counter: " + counter);
			System.out.println("Total time elapsed: " +
			                    stopWatch.timeElapsedMillis() +
			                    " milliseconds");
		}
	}
	
	private static void printTitle(String title) {
		System.out.println("*********** " + title + " ***************");
	}
	
	public static void main(String[] args) {
		printTitle("No Synchronization");
		NoSync.runTwoIncrementers();
		printTitle("Synchronized Method");
		SynchronizedMethod.runTwoIncrementers();
		printTitle("Atomic Integer");
		Atomic.runTwoIncrementers();
		printTitle("Reentrant Lock");
		ReentLock.runTwoIncrementers();
	}
}
