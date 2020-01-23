package il.co.ilrd.crud;

import java.io.Serializable;

public interface CRUD<K extends Serializable, E extends Serializable> extends AutoCloseable {
	
	/*
	 * creates a new entry, returns a key to that entry
	 */
	public K create(E entry);
	
	/*
	 * returns an entry that matches that key (or null)
	 */
	public E read(K key);
	
	/*
	 * replaces the entry that matches the given key with newEntry
	 */
	public void update(K key, E newEntry);	
	
	/*
	 * deletes the entry that matches the key
	 */
	public void delete(K key);
}


