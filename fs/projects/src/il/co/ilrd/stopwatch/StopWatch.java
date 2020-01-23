package il.co.ilrd.stopwatch;

import java.util.concurrent.TimeUnit;

public class StopWatch {
	private long startTime;
	private boolean running = false;
	private long timeElaspsed;

	/*
	 * Starts the stop watch
	 * @throws RuntimeException if the stop watch is currently running
	 */
	public void start() {
		if(isRunning()) {
			throw new RuntimeException("Can't start stopwatch twice");
		}
		
		startTime = System.nanoTime();
		running = true;
		timeElaspsed = 0;
	}
	
	/*
	 * Stops the stop watch
	 * @throws RuntimeException if the stop watch wasn't running
	 * @returns the number of milliseconds the stop watch spent running
	 */
	public long stop() {
		if(!isRunning()) {
			throw new RuntimeException("Can't stop a stopwatch that wasn't started");
		}

		running = false;
		timeElaspsed = System.nanoTime() - startTime;
		
		return timeElapsedMillis();
	}
	
	/*
	 * Stops the stop watch
	 * @throws RuntimeException if the stop watch wasn't running
	 * @returns the number of milliseconds the stop watch spent running the last
	 * time it worked
	 */
	public long timeElapsedMillis() {	
		return TimeUnit.NANOSECONDS.toMillis(timeElaspsed);
	}
	
	/*
	 * @returns the number of nanoseconds the stop watch spent running the last
	 * time it worked
	 */
	public long timeElapsedNano() {
		return timeElaspsed;
	}
	
	/*
	 * @returns true if the stop watch is currently running, false otherwise
	 */
	public boolean isRunning() {
		return running;
	}
}