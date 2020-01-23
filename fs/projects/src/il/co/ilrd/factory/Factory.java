package il.co.ilrd.factory;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

/*
 * K is the type of the key.
 * D is the type that holds the arguments for instance creation.
 * T is the type of object the factory produces. 
 */

public class Factory<K, D, T> {
	private Map<K, Function<D, ? extends T>> functionsMap = new HashMap<>();
	
	public void add(K key, Function<D, ? extends T> function) {
		functionsMap.put(key, function);
	}
	
	public T create(K key) {
		Function<D, ? extends T> f = functionsMap.get(key);
		
		if(null == f) { throw new IllegalArgumentException(); }
		
		return f.apply(null);
	}
	
	public T create(K key, D args) {
		return functionsMap.get(key).apply(args);
	}
}