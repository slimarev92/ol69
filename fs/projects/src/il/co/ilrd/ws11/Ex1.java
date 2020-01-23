package il.co.ilrd.ws11;

class StartStopThread extends Thread {
	private volatile boolean shouldStop = false;
	
	@Override
	public void run() {
		while(!shouldStop) {
			System.out.println("Doing the task");
			
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}		
		}
		
		System.out.println("Stopping the task");
		System.out.println("*******************");
	}
	
	public void stopTask() {
		this.shouldStop = true;
	}
}

class StartStopRunnable implements Runnable {
	private volatile boolean shouldStop = false;

	@Override
	public void run() {
		while(!shouldStop) {
			System.out.println("Doing the task");
			
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}		
		}
		
		System.out.println("Stopping the task");	
	}
	
	public void stopTask() {
		this.shouldStop = true;
	}
}

public class Ex1 {
	public static void main(String[] args) {
		/* bullet one */
		StartStopThread th = new StartStopThread();
		
		th.start();
		
		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}	
		
		th.stopTask();		
		
		/* bullet two */
		StartStopRunnable rn = new StartStopRunnable();
		Thread th2 = new Thread(rn);
		
		th2.start();
	
		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}	
		
		rn.stopTask();
	}
}
