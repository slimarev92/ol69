package il.co.ilrd.ws11;

import java.util.Arrays;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class Ex3 {

	@SuppressWarnings("unused")
	private static class PingPong {
		private static final AtomicBoolean isPing = new AtomicBoolean(true);
		private static final Participant ping = new Participant("1 - ping", true);
		private static final Participant pong = new Participant("2 - pong\n**********", false);
		
		private static class Participant extends Thread {
			private final String message;
			private final boolean printOnCondition;

			public Participant(String message, boolean printOnCondition) {
				this.message = message;
				this.printOnCondition = printOnCondition;
			}

			@Override
			public void run() {
				while(true) {
					synchronized(isPing) {
						if(printOnCondition == isPing.get()) {
							System.out.println(message);
							isPing.set(!printOnCondition);
							isPing.notify();
						} else {
							try {
								isPing.wait();
							} catch (InterruptedException e) { e.printStackTrace(); }
						}
					}
				}	
			}
		}
		
		@SuppressWarnings("unused")
		private static void runPingPong() {
			ping.start();
			pong.start();
		}
	}
	
	@SuppressWarnings("unused")
	private static class ProducerConsumer {
		private final int[] buffer = new int[BUFFER_SIZE];
		private int currIndexRead = 0;
		private int currIndexWrite = 0;
		private final int numProducers;
		private final int numConsumers;
		private static final int BUFFER_SIZE = 5;
		private final Semaphore tasksInBuffer = new Semaphore(0);
		private final Semaphore remainingCapacity = new Semaphore(BUFFER_SIZE);
		
		@SuppressWarnings("unused")
		ProducerConsumer(int numProducers, int numConsumers) {
			this.numConsumers = numConsumers;
			this.numProducers = numProducers;
			Arrays.fill(buffer, -10);
		}
			
		private class Producer extends Thread {
			@Override
			public void run() {
				while(true) {
					int product = produce();
					
					try {
						remainingCapacity.acquire();
					} catch (InterruptedException e) { e.printStackTrace(); }
					
					synchronized(buffer) {
						System.out.println("Inserting product " + 
					                       product + 
					                       " at index " + 
					                       currIndexWrite);
						buffer[currIndexWrite] = product;
						currIndexWrite = (currIndexWrite + 1) % BUFFER_SIZE;
					}
					
					tasksInBuffer.release();	
				}
			}
			
			private int produce() {
				int product = (int)(Math.random() * 1000);
					
//				try {
//					Thread.sleep(2000);
//				} catch (InterruptedException e) {
//					e.printStackTrace();
//				}
				
				return product;
			}
		}	
		
		private class Consumer extends Thread {
			
			@Override
			public void run() {
				while(true) {
					try {
						tasksInBuffer.acquire();
					} catch(InterruptedException e) { e.printStackTrace(); }
					
					synchronized(buffer) {
						consume(buffer[currIndexRead]);
						buffer[currIndexRead] = -10;
						currIndexRead = (currIndexRead + 1) % BUFFER_SIZE;
					}	
					
					remainingCapacity.release();
				}
			}
			
			private void consume(int product) {
				System.out.println("Consuming product " + 
			                       product + 
			                       " at index " + 
			                       currIndexRead);
				
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
		
		@SuppressWarnings("unused")
		private void runProducersConsumers() {
			
			for(int i = 0; i < numConsumers; ++i) {
				new Consumer().start();
			}
			
			for(int i = 0; i < numProducers; ++i) {
				new Producer().start();
			}
		}	
	}
	
	private static class WithBarrier {
		private final int numConsumers;
		private final StringHolder messageHolder = new StringHolder();
		private final Lock messageLock = new ReentrantLock();
		private final Condition messageReadyCondition = messageLock.newCondition();
		private final Semaphore numReadyConsumers = new Semaphore(0);
		private static int messageCounter = 0;
	
		public WithBarrier(int numConsumers) {
			this.numConsumers = numConsumers;
			
			for(int i = 0; i < numConsumers; ++i) {
				new Consumer().start();
			}
			
			new Producer().start();
		}
		
		private class Producer extends Thread {
			private boolean mayProceed = false;
			
			@Override
			public void run() {
				while(true) {
					while(!mayProceed) {
						try {
							numReadyConsumers.acquire(numConsumers);
							mayProceed = true;
						} catch (InterruptedException e) { e.printStackTrace(); }
					}
					
					messageLock.lock();
					
					messageHolder.setMessage(messageCounter + "");
					messageReadyCondition.signalAll();
					
					messageLock.unlock();
					
					++messageCounter;
					mayProceed = false;
				}
			}
		}
		
		private class Consumer extends Thread {
			private boolean mayProceed = false;
			
			@Override
			public void run() {
				while(true) {
					messageLock.lock();
					
					numReadyConsumers.release();
						
					while(!mayProceed) {
						try {
							messageReadyCondition.await();
							mayProceed = true;
						} catch (InterruptedException e) { e.printStackTrace(); }
					}
					
					messageLock.unlock();
					
					System.out.println("Printing message " + messageHolder.getMessage());
					mayProceed = false;
				}
			}
		}
		
		private static class StringHolder {
			private String message;
			
			private void setMessage(String message) {
				this.message = message;
			}
			
			private String getMessage() {
				return message;
			}
		}
	}
	
	
	public static void main(String[] args) {
		//PingPong.runPingPong();
		
		//ProducerConsumer pc = new ProducerConsumer(2, 1);
		//pc.runProducersConsumers();
		
		@SuppressWarnings("unused")
		WithBarrier wb = new WithBarrier(5);
	}
}
