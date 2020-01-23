package il.co.ilrd.networking;

import java.io.IOException;

public interface ServerAdapter {
	public Message receive(int size) throws IOException; 
	public void close();
}