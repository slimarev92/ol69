package il.co.ilrd.hashmap;

import java.util.AbstractCollection;
import java.util.AbstractSet;
import java.util.ArrayList;
import java.util.Collection;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import il.co.ilrd.pair.Pair;

public class SashMap<K, V> implements Map<K, V> {
	private List<List<Map.Entry<K, V>>> mapBuckets;
	private Set<Map.Entry<K, V>> mapEntrySet;
	private Set<K> mapKeySet;
	private Collection<V> mapValues;
	private final int capacity;
	private int modCount = 0;
	private static final int DEFAULT_CAPACITY = 16;
	private static final int NULL_BUCKET = 0;
	
	public SashMap() {
		this(DEFAULT_CAPACITY);
	}
	
	public SashMap(int capacity) {
		if(0 >= capacity) {
			throw new IllegalArgumentException("Capacity must be non negative");
		}
		
		this.capacity = capacity;
		mapBuckets = new ArrayList<List<Entry<K,V>>>();
		
		/* to prevent throwing of NullPointerExcpetion by entry iterators */
		addBucketsIfNeeded(0);
	}

	@Override
	public void clear() {
		Iterator<List<Map.Entry<K, V>>> iter = mapBuckets.iterator();
		
		while(iter.hasNext()) {
			iter.next().clear();
		}
			
		increaseModCount();
	}

	@Override
	public boolean containsKey(Object key) {
		return (null != findEntry(key, calculateBucket(key)));
	}

	@Override
	public boolean containsValue(Object value) {
		for(V currValue : values()) {
			if(safeEquals(currValue, value)) {
				return true;
			}
		}
		
		return false;
	}

	@Override
	public Set<Map.Entry<K, V>> entrySet() {
		if(null == mapEntrySet) { mapEntrySet = new EntrySet(); }
		
		return mapEntrySet;
	}

	@Override
	public V get(Object key) {
		Map.Entry<K, V> entry = findEntry(key, calculateBucket(key));
			
		return (null == entry) ? null : entry.getValue();
	}

	@Override
	public boolean isEmpty() {
		return (!entrySet().iterator().hasNext());
	}

	@Override
	public Set<K> keySet() {
		if(null == mapKeySet) { mapKeySet = new KeySet(); }
		
		return mapKeySet;
	}

	@Override
	public V put(K key, V value) {
		int bucket = calculateBucket(key);

		addBucketsIfNeeded(bucket);
		
		Map.Entry<K, V> entry = findEntry(key, bucket);
		V retVal = null;
		
		if(null != entry) {	
			retVal = entry.setValue(value);
		} else {
			increaseModCount(); 
			mapBuckets.get(bucket).add(Pair.of(key, value));	
		}
		
		return retVal;
	}

	@Override
	public void putAll(Map<? extends K, ? extends V> sourceMap) {
		for(Map.Entry<? extends K, ? extends V> currEntry : sourceMap.entrySet()) {
			put(currEntry.getKey(), currEntry.getValue());
		}
	}

	@Override
	public V remove(Object key) {
		int bucket = calculateBucket(key);
		
		if(!bucketExists(bucket)) { return null; }
		
		Iterator<Map.Entry<K, V>> iter = mapBuckets.get(bucket).iterator();
		
		while(iter.hasNext()) {
			Map.Entry<K, V> curr = iter.next();
			
			if(safeEquals(curr.getKey(), key)) {
				iter.remove();
				increaseModCount();
				
				return curr.getValue();
			}
		}
		
		return null;
	}

	@Override
	public int size() {		
		return entrySet().size();
	}

	@Override
	public Collection<V> values() {
		if(null == mapValues) { mapValues = new Values(); }
		
		return mapValues;
	}
	
	private void increaseModCount() {
		++modCount;
	}
	
	private int calculateBucket(Object key) {
		return (null != key) ?  Math.abs(key.hashCode() % capacity) : 
			                    NULL_BUCKET;			                  
	}
	
	private void addBucketsIfNeeded(int bucket) {
		int numIters = (bucket + 1) - mapBuckets.size();
		
		for(int i = 0; i < numIters; ++i) {
			mapBuckets.add(new ArrayList<Map.Entry<K,V>>());
		}
	}
	
	private boolean bucketExists(int bucket) {
		return (bucket <= mapBuckets.size() - 1);
	}
	
	private Map.Entry<K, V> findEntry(Object key, int bucket) {	
		if(!bucketExists(bucket)) {
			return null;
		}
		
		for(Map.Entry<K, V> currEntry : mapBuckets.get(bucket)) {
			if(safeEquals(currEntry.getKey(), key)) {
				return currEntry;
			}
		}
		
		return null;
	}
	
	private boolean safeEquals(Object invokeTarget, Object invokeParam) {
		return null == invokeTarget ? invokeTarget == invokeParam :
					   invokeTarget.equals(invokeParam);
	}
	
	private class EntrySet extends AbstractSet<Map.Entry<K, V>> { 

		@Override
		public Iterator<Entry<K, V>> iterator() {
			return new EntryIterator();
		}

		@Override
		public int size() {
			int result = 0;
			Iterator<Entry<K, V>> iter = iterator();
			
			while(iter.hasNext()) {
				++result;
				iter.next();
			}
			
			return result;
		}
		
		private class EntryIterator implements Iterator<Entry<K, V>> {
			private Iterator<List<Map.Entry<K, V>>> bucketsIter;
			private Iterator<Entry<K, V>> currBucket;
			private final int expectedModCount = modCount;

			public EntryIterator() {
				bucketsIter = mapBuckets.iterator();
				currBucket = bucketsIter.next().iterator();
			}

			@Override
			public boolean hasNext() {
				advanceToFirstNonEmptyBucket();

				return currBucket.hasNext();
			}

			@Override
			public Entry<K, V> next() {
				advanceToFirstNonEmptyBucket();

				return currBucket.next();
			}

			private void advanceToFirstNonEmptyBucket() {
				checkModification();

				while (bucketsIter.hasNext() && !currBucket.hasNext()) {
					currBucket = bucketsIter.next().iterator();
				}
			}

			private void checkModification() {
				if (expectedModCount != modCount) {
					throw new ConcurrentModificationException();
				}
			}
		}
	}
	
	private class KeySet extends AbstractSet<K> {

		@Override
		public Iterator<K> iterator() {
			return new KeyIterator();
		}

		@Override
		public int size() {
			return entrySet().size();
		}
		
		private class KeyIterator implements Iterator<K> {
			private Iterator<Map.Entry<K, V>> entries = entrySet().iterator();

			@Override
			public boolean hasNext() {
				return entries.hasNext();
			}

			@Override
			public K next() {
				return entries.next().getKey();
			}
		}
	}

	private class Values extends AbstractCollection<V> {

		@Override
		public Iterator<V> iterator() {
			return new ValueIterator();
		}

		@Override
		public int size() {
			return entrySet().size();
		}
		
		private class ValueIterator implements Iterator<V> {
			private Iterator<Map.Entry<K, V>> entries = entrySet().iterator();

			@Override
			public boolean hasNext() {
				return entries.hasNext();
			}

			@Override
			public V next() {
				return entries.next().getValue();
			}
		}
	}
}