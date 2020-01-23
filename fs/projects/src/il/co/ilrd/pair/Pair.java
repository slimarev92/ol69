package il.co.ilrd.pair;

import java.util.Comparator;
import java.util.Map;

/*
 * @author Sasha Limarev
 * @version 25/09/19
 * Pairs with keyS of type K and valueS of type V.
 */

public class Pair<K,V> implements Map.Entry<K,V>  {
	private K key;
	private V value;

	private Pair(K key, V value) {
		this.key = key;
		this.value = value;
	}

	public static <Q,W> Pair<Q,W> of(Q key, W value) {		
		return new Pair<>(key, value);
	}

	@Override
	public K getKey() {
		return key;
	}
	
	@Override
	public V getValue() {
		return value;
	}
	
	public K setKey(K key) {
		K oldKey = this.key;
		this.key = key;
		
		return oldKey;
	}
	
	@Override
	public V setValue(V value) {
		V oldValue = this.value;
		this.value = value;
		
		return oldValue;
	}
	
	@Override
	public String toString() {
		return "Key: " + key + ", Value: " + value;
	}
	
	@Override
	public boolean equals(Object obj) {
		if(!(obj instanceof Pair)) {
			return false;
		}
		
		Pair<?, ?> objAsPair = (Pair<?, ?>)obj;
		boolean keyCheck = (null == key) ? 
			                               null == objAsPair.getKey() :
				                           getKey().equals(objAsPair.getKey());
		boolean valueCheck = (null == key) ? 
										null == objAsPair.getValue() : 
									    getValue().equals(objAsPair.getValue());
										
		return keyCheck && valueCheck;
	}
	
	@Override
	public int hashCode() {
		int keyHash = (null == key) ? 0 : key.hashCode();
		int valueHash = (null == value) ? 0 : value.hashCode();
		
		return keyHash ^ valueHash;
	}

	public static <T> Pair<T,T> minMax(T[] array, Comparator<? super T> comp) {
		T minCandidate = array[0];
		T maxCandidate = array[0];
		
		for(int i = 1; array.length > i; ++i) {
			if(0 <= comp.compare(minCandidate, array[i])) {
				minCandidate = array[i];
			} else if(0 < comp.compare(array[i], maxCandidate)) {
				maxCandidate = array[i];
			}
		}
		
		return Pair.of(minCandidate, maxCandidate);
	}	
	
	public static <T extends Comparable<? super T>> Pair<T,T> minMax(T[] array) {	
		return Pair.minMax(array, (T first, T second) -> first.compareTo(second));
	}
}