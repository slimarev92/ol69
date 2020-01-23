package il.co.ilrd.concurrency;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.CancellationException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.ReentrantLock;
import il.co.ilrd.stopwatch.StopWatch;

public class ThreadPool implements Executor {
	private final WaitableQueue<Task<?>> taskQueue = new WaitableQueueSemaphore<>();
	private final List<Worker> workers = new LinkedList<>();
	private volatile CountDownLatch pauseSwitch;
	private boolean isRunning = true;
	private boolean isPaused = false;
	private int numOfThreads;
	
	private static final int TOP_PRIORITY = Priority.HIGH.getPriority() + 2;
	private static final int HIGHER_PRIORITY = Priority.HIGH.getPriority() + 1;
	private static final int BOTTOM_PRIORITY = Priority.LOW.getPriority() -1;
	private static final Priority DEFAULT_PRIORITTY = Priority.MEDIUM;
	private static final int DEFAULT_NUM_OF_THREADS = 4;
	private static final int OFF = 1;
	
    public ThreadPool() {
    	this(DEFAULT_NUM_OF_THREADS);
    }
    
    public ThreadPool(int numOfThreads) {
    	if(0 >= numOfThreads) {
    		throw new IllegalArgumentException("Number of threads must be positive"); 
    	}
       
    	this.numOfThreads = numOfThreads;
    	addWorkers(numOfThreads);
    }
    
    public <T> Future<T> submit(Callable<T> callable, Priority priority) {
    	if(!isRunning) { throw new RejectedExecutionException(); }
    	else if(null == callable) { throw new IllegalArgumentException("Callable can't be null"); }
    	
    	Task<T> task = new Task<>(callable, priority.getPriority());
    	taskQueue.enqueue(task);
    	
        return task.getFuture();
    }
    
    public <T> Future<T> submit(Callable<T> callable) {
    	return submit(callable, DEFAULT_PRIORITTY);
    }
    
    public Future<?> submit(Runnable runnable, Priority priority) {   	
    	return submit(runnable, priority, null);
    }
    
    public <T> Future<T> submit(Runnable runnable, Priority priority, T result) {
    	if(null == runnable) { throw new IllegalArgumentException("runnable can't be null"); }
    	
    	return submit(Executors.callable(runnable, result), priority);
    }
    
    @Override
    public void execute(Runnable runnable) {
    	submit(runnable, DEFAULT_PRIORITTY);
    }
    
    public void setNumThreads(int numOfThreads) {
		if(0 > numOfThreads) {
			throw new IllegalArgumentException("Number of threads must be non-negative");
		} else if(!isRunning) {
			throw new RuntimeException("Thread pool was shut down");
		}
		
    	cleanTermindatedWorkers();
    	
    	int difference = numOfThreads - this.numOfThreads;
    	this.numOfThreads = numOfThreads;
    	
    	if(0 <= difference) {
    		if(isPaused) { submitPauseTasks(difference); }
    		addWorkers(difference);
    	} else {
    		shutdownWorkersNow(Math.abs(difference));
    	}
    }
    
    public void pause() {
    	if(null != pauseSwitch) { throw new RuntimeException("Can't pause twice"); }
    	if(isPaused) { throw new IllegalStateException("Can't pause twice"); }
    	
    	isPaused = true;
    	pauseSwitch = new CountDownLatch(OFF);

    	submitPauseTasks(numOfThreads);
    }
    
    public void resume() {
    	if(null == pauseSwitch) { throw new RuntimeException("Can't resume unpaused pool"); }
    	
    	isPaused = false;
    	pauseSwitch.countDown();
    }
    
    public void shutdown() {
    	if(!isRunning) { throw new RuntimeException("Can't shutdown twice"); }
    	
    	cleanTermindatedWorkers();
    	
    	shutdownWorkers(numOfThreads);
    	isRunning = false;
    }
    
    public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
    	if(isRunning) { throw new RuntimeException("Thread Pool wasn't shut down"); }

    	long remainingTime = unit.toMillis(timeout);    
    	StopWatch stopWatch = new StopWatch();
    	
    	for(Thread curr : workers) {
    		if(0 >= remainingTime) { break; }
    		
    		stopWatch.start();

    		if(curr.isAlive()) { curr.join(remainingTime); }
    		
    		remainingTime -= stopWatch.stop();
    	}
    	
        return (0 < remainingTime);
    }
      
  
    private void addWorkers(int amount) { 	
		for (int i = 0; amount > i; ++i) {
			Worker currWorker = new Worker();
			currWorker.start();
			workers.add(currWorker);
		}
    }
       
    private void shutdownWorkersNow(int amount) {
    	final Callable<?> callable = new Callable<Void> () {
			@Override
			public Void call() throws Exception {
				throw new WorkerTerminatedException("Worker terminated");
			}
    	};
    	
    	for(int i = 0; amount > i; ++i) {
    	 	Task<?> task = new Task<>(callable, TOP_PRIORITY);
        	taskQueue.enqueue(task);
    	}
    }
    
    private void shutdownWorkers(int amount) {
    	final Callable<?> callable = new Callable<Void> () {
			@Override
			public Void call() throws Exception {
				throw new WorkerTerminatedException("Worker terminated");
			}
    	};
    	
    	for(int i = 0; amount > i; ++i) {
    	 	Task<?> task = new Task<>(callable, BOTTOM_PRIORITY);
        	taskQueue.enqueue(task);
    	}
    }
    
    private void submitPauseTasks(int numTasks) {
    	Callable<?> callable = new Callable<Void>() {

			@Override
			public Void call() throws Exception {
				pauseSwitch.await();
				return null;
			}
    	};
    
    	for(int i = 0; numTasks > i; ++i) {
    		Task<?> task = new Task<>(callable, HIGHER_PRIORITY);
        	taskQueue.enqueue(task);
		}
    }
    
    private void cleanTermindatedWorkers() {
		workers.removeIf(worker -> !worker.isAlive());
    }
    
    private class Worker extends Thread {
    	private boolean isRunning = true;
    	
		@Override
		public void run() {
			while(isRunning) {
				try {
					Task<?> currTask = taskQueue.dequeue();
					currTask.execute();
				} catch(InterruptedException e) {
					e.printStackTrace();
				} catch(WorkerTerminatedException e) {
					isRunning = false;						
				} 
			}
		}
    }     
       
    private static class Task<T> implements Comparable<Task<?>> {
    	private final Callable<T> callable;
    	private final TaskFuture<T> future;
    	private final int priority;

    	public Task(Callable<T> callable, int priority) {
            this.callable = callable;
            this.priority = priority;
          
            future = new TaskFuture<>();
        }
    	      
    	private Future<T> getFuture() {
    		return future;
    	}
    	
        private void execute() throws WorkerTerminatedException {
        	future.getCancelLock();
        	
        	try {
        		if(!future.isCancelled()) {
        			future.setTaskStatus(TaskStatus.IN_PROGRESS);
        		} else {
        			return;
        		}
        	} finally {
        		future.releaseCancelLock();
        	}
        	
        	try {		
        		future.setResult(callable.call());
        	} catch(WorkerTerminatedException e) {
        		throw e;
			} catch(Exception e) { 
				ExecutionException execException = 
						new ExecutionException("An exception occured during execution", e);
				future.setExecutionException(execException);
			} finally {
				future.taskDone();
			}
        }
        
        @Override
        public int compareTo(Task<?> other) {
        	return other.priority - priority;        		
        }
        
        private static class TaskFuture<T> implements Future<T> {
        	private final CountDownLatch doneSwitch = new CountDownLatch(OFF);
        	private final ReentrantLock cancelLock = new ReentrantLock();
        	private volatile TaskStatus taskStatus = TaskStatus.WAITING;
        	private ExecutionException executionException;
        	private volatile boolean cancelled = false;    	
        	private volatile T result;
              	
            @Override
            public boolean cancel(boolean mayInterruptIfRunning) {
            	if(isCancelled()) { return false; }
            	
            	getCancelLock();

				try {
					if(TaskStatus.WAITING == taskStatus) {
						cancelled = true;
						taskDone();
					}
				} finally {
					releaseCancelLock();
				}
            	    	
                return isCancelled();
            }

            @Override
            public T get() throws InterruptedException, ExecutionException {	
                try {
                	return get(Long.MAX_VALUE, TimeUnit.DAYS);
				} catch (TimeoutException e) { 
					/* This can only happen if something is wrong with the 
					 * JVM or the machine it's running on. 
					 */
					throw new UnknownError("A serious error has occured in the JVM"); 
				} 
            }

            @Override
            public T get(long timeout, TimeUnit unit)
                    throws InterruptedException, ExecutionException, TimeoutException {
				if(isCancelled()) {
					throw new CancellationException("Task was cancelled");
				} else {
					if(false == doneSwitch.await(timeout, unit)) {
						throw new TimeoutException("Task wasn't completed in time");
					} else if(null != executionException) {
						throw executionException;
					} 
				}
            
            	return result;
            }

            @Override
            public boolean isCancelled() {
            	return cancelled;
            }

            @Override
            public boolean isDone() {
            	return (TaskStatus.DONE == taskStatus);
            } 
            
            private void setExecutionException(ExecutionException executionException) {
            	this.executionException = executionException;
            }
                       
        	private void setResult(T result) {
        		this.result = result;
        	}
        	
        	private void setTaskStatus(TaskStatus taskStatus) {
        		this.taskStatus = taskStatus;
        	}
        
        	private void taskDone() {
        		setTaskStatus(TaskStatus.DONE);
        		doneSwitch.countDown();		
        	}
        	
        	private void getCancelLock() {
        		cancelLock.lock();
        	}
        	
        	private void releaseCancelLock() {
        		cancelLock.unlock();
        	}
        } /* CLASS TaskFuture */
        
        private enum TaskStatus { WAITING, IN_PROGRESS, DONE };    
    } /* CLASS Task */
    
    public enum Priority {
        HIGH(10), MEDIUM(5), LOW(1);
        
        private final int priority;
        
        private Priority(int priority) {
            this.priority = priority;
        }
        
        public int getPriority() {
            return priority;
        }
    } /* ENUM Priority */
    
	private class WorkerTerminatedException extends Exception {
		private static final long serialVersionUID = 1L;

		public WorkerTerminatedException(String message) { 
    		super(message);
    	}
    } /* CLASS WorkerTerminatedException */
} /* CLASS ThreadPool */