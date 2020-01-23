package il.co.ilrd.concurrency;

import java.util.concurrent.TimeUnit;

import java.util.concurrent.TimeoutException;

public interface WaitableQueue<T> {
	
    public void enqueue(T item);

    /**
     * Blocking until there is an item to dequeue
     * @return
     */
    public T dequeue() throws InterruptedException;


    /**
     * Blocking until there is an item to dequeue or until TimeOut.
     * @return
     */
    public T dequeue(long timeout, TimeUnit timeUnit) throws TimeoutException, InterruptedException;

    public boolean remove(T item);
}

