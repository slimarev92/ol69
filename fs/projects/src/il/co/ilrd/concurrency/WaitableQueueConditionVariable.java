package il.co.ilrd.concurrency;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueueConditionVariable<E> implements WaitableQueue<E> {
    private PriorityQueue<E> queue;
    private final ReentrantLock queueLock = new ReentrantLock();
    private final Condition queueCondition = queueLock.newCondition();

    public WaitableQueueConditionVariable() {
    	queue = new PriorityQueue<E>();
    }
    
    public WaitableQueueConditionVariable(Comparator<? super E> comparator) {
    	queue = new PriorityQueue<E>(comparator);
    }

	@Override
	public void enqueue(E item) {
		queueLock.lock();
		
		try {
			queue.add(item);
			queueCondition.signal();
		} finally {
			queueLock.unlock();
		}
	}

	@Override
	public E dequeue() throws InterruptedException {
		queueLock.lock();
		
		try {
			while(queue.isEmpty()) {
				queueCondition.await();				
			}
			
			return queue.poll();
		} finally {
			queueLock.unlock();
		}
	}

	@Override
	public E dequeue(long timeout, TimeUnit timeUnit) throws TimeoutException, InterruptedException {
		queueLock.lock();
		
		try {
			while(queue.isEmpty()) {				
				if(false == queueCondition.await(timeout, timeUnit)) {
					throw new TimeoutException();
				}
			}
	
			return queue.poll();
		} finally {
			queueLock.unlock();
		}
	}

	@Override
	public boolean remove(E item) {
		queueLock.lock();
		
		try {
			return queue.remove(item);
		} finally {
			queueLock.unlock();
		}
	}
}