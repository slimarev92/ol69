package il.co.ilrd.observer;

import java.util.LinkedList;
import java.util.List;

public class Dispatcher<T> {
	private final List<Callback<? super T>> callbacks = new LinkedList<>();
	
	public void subscribe(Callback<? super T> callback) {
		callbacks.add(callback);
		callback.setDispatcher(this);
	}
	
	public void unsubscribe(Callback<? super T> callback) {
		callback.setDispatcher(null);
		callbacks.remove(callback);
	}
	
	public void notify(T data) {
		callbacks.forEach(e -> e.update(data));
	}
	
	public void stop() {
		for(Callback<? super T> curr : callbacks) {
			curr.stop();
			curr.setDispatcher(null);
		}
	}
}
