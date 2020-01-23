package il.co.ilrd.ws12;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import il.co.ilrd.stopwatch.StopWatch;

public class Ex2 {
	
	public static void timeExecution(Runnable task) {
		StopWatch stopWatch = new StopWatch();
		
		stopWatch.start();
		task.run();
		System.out.println("Total time elapsed: " + stopWatch.stop());
	}

	private static class NumberedTask implements Runnable {
		private final int taskNum;
		private final Semaphore tasksDone;
		
		public NumberedTask(int taskNum, Semaphore tasksDone) {
			this.taskNum = taskNum;
			this.tasksDone = tasksDone;
		}
		
		@Override
		public void run() {
			try {
				Thread.sleep(1000);
				System.out.println("Performing task " + taskNum);
				tasksDone.release();
			} catch (InterruptedException e) {
				System.out.println("Task " + taskNum + " was cancelled");
			}
		}
	}
	
	private static class singlePool implements Runnable {
		@Override
		public void run() {
			Semaphore tasksDone = new Semaphore(0);
			ExecutorService singleThreadExecutor = Executors.newSingleThreadExecutor();
	
			for(int i = 0; 5 > i; ++i) {
				singleThreadExecutor.submit(new NumberedTask(i, tasksDone));
			}
			
			try {
				tasksDone.acquire(5);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static class fixedPool implements Runnable {
		@Override
		public void run() {
			Semaphore tasksDone = new Semaphore(0);
			ExecutorService fixedThreadExecutor = Executors.newFixedThreadPool(3);
	
			for(int i = 0; 5 > i; ++i) {
				fixedThreadExecutor.submit(new NumberedTask(i, tasksDone));
			}
			
			try {
				tasksDone.acquire(5);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static class cachedPool implements Runnable {
		@Override
		public void run() {
			Semaphore tasksDone = new Semaphore(0);
			ExecutorService cachedThreadExecutor = Executors.newCachedThreadPool();
	
			for(int i = 0; 5 > i; ++i) {
				cachedThreadExecutor.submit(new NumberedTask(i, tasksDone));
			}
			
			try {
				tasksDone.acquire(5);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static class scheduledPool implements Runnable {
		@Override
		public void run() {
			Semaphore tasksDone = new Semaphore(0);
			ScheduledExecutorService scheduledThreadExecutor = Executors.newSingleThreadScheduledExecutor();
	
			for(int i = 0; 5 > i; ++i) {
				scheduledThreadExecutor.schedule(new NumberedTask(i, tasksDone),
						                         1,
						                         TimeUnit.SECONDS);
			}
			
			try {
				tasksDone.acquire(5);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void main(String[] args) {

		System.out.println("***********Single thread pool*************");
		timeExecution(new singlePool());	
		System.out.println("***********Fixed thread pool**************");
		timeExecution(new fixedPool());	
		System.out.println("***********Cached thread pool**************");
		timeExecution(new cachedPool());	
		System.out.println("***********Scheduled thread pool***********");
		timeExecution(new scheduledPool());
	}
}
