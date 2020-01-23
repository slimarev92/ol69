package il.co.ilrd.networking;

import java.io.IOException;

public interface Message {
	public void reply(byte[] data) throws IOException; 
	public byte[] getData();
}
