package il.co.ilrd.ws12;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Ex1 {
	public static void main(String[] args) {
		ExecutorService ex = Executors.newFixedThreadPool(5);
		
		Callable<Void> task = new Callable<Void>() {
			@Override
			public Void call() {
				try {
					Thread.sleep(1500);
					System.out.println("Performing task");
				} catch (InterruptedException e) { System.out.println("Task was cancelled"); }
				
				
				return null;
			}
		};
		
		/* Successful completion of task */
			
		try {
			Future<?> f = ex.submit(task);
			
			Thread.sleep(500);
			System.out.println("Status after 500 millis " + f.isDone());
			Thread.sleep(500);
			System.out.println("Status after 1000 millis " + f.isDone());
			Thread.sleep(500);
			System.out.println("Status after 1500 millis " + f.isDone());
		} catch(InterruptedException e) {
			e.printStackTrace();
		}
		
		System.out.println("*************************************");
		
		/* Task is cancelled before completion */
		
		try {
			Future<?> f = ex.submit(task);
			
			Thread.sleep(500);
			System.out.println("Status after 500 millis " + f.isDone());
			Thread.sleep(500);
			System.out.println("Status after 1000 millis " + f.isDone());
			System.out.println("Cancelling task");
			f.cancel(true);
			Thread.sleep(500);
			System.out.println("Status after 1500 millis " + f.isDone());
		} catch(InterruptedException e) {
			e.printStackTrace();
		}
		
	}
}
