package il.co.ilrd.observer;

import java.util.function.Consumer;

public class Callback<T> {
	private Consumer<T> onUpdate;
	private Worker onStop;
	private Dispatcher<? extends T> dispatcher;
	
	public Callback(Consumer<T> onUpdate, Worker onStop) {
		if(null == onUpdate || null == onStop) {
			throw new IllegalArgumentException();
		}
		
		this.onUpdate = onUpdate;
		this.onStop = onStop;
	}
	
	public void setDispatcher(Dispatcher<? extends T> dispatcher) {
		this.dispatcher = dispatcher;
	}
	
	public void update(T data) {
		onUpdate.accept(data);
	}
	
	public void stop() {
		onStop.doWork();
	}
	
	public void unsubscribe() {	
		dispatcher.unsubscribe(this);
	}
}