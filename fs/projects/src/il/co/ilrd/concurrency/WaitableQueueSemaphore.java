package il.co.ilrd.concurrency;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueSemaphore<E> implements WaitableQueue<E> {
	private PriorityQueue<E> queue;
	private Lock queueLock = new ReentrantLock();
	private Semaphore itemsInQueue = new Semaphore(0);

    public WaitableQueueSemaphore() {
        queue = new PriorityQueue<>();
    }

    public WaitableQueueSemaphore(Comparator<? super E> comparator) {
    	queue = new PriorityQueue<>(comparator);
    }

    @Override
    public void enqueue(E item) {
    	queueLock.lock();
    	
    	try {
    		queue.add(item);
    		itemsInQueue.release();
    	} finally {
    		queueLock.unlock();
    	}
    }

    @Override
    public E dequeue() throws InterruptedException {
    	itemsInQueue.acquire();
    	queueLock.lock();  
        
        try {
    		return queue.poll();
    	} finally {
    		queueLock.unlock();
    	}
    }

    @Override
    public E dequeue(long timeout, TimeUnit timeUnit) throws TimeoutException, InterruptedException {	
		if(false == itemsInQueue.tryAcquire(timeout, timeUnit)) {
			throw new TimeoutException();
		}
		
		queueLock.lock();
				
        try {
        	return queue.poll();	
        } finally {
        	queueLock.unlock();
        }
    }

    @Override
    public boolean remove(E item) {    	
    	queueLock.lock();
    	
        try {
        	boolean itemRemoved = false;
        	
        	if(true == itemsInQueue.tryAcquire()) {
        		itemRemoved = queue.remove(item);
        		
        		if(false == itemRemoved) {   		
            		itemsInQueue.release();
            	}
        	}
        	
        	return itemRemoved; 
        } finally {
        	queueLock.unlock();
        }
    }
}