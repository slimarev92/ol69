package il.co.ilrd.networking;

import java.io.IOException;

public interface ServerProtocol {
	/* RETURNS NEXT UNIQUE MESSAGE, HANDLES ACKNOWLEDGEMENT OF INCOMING MESSAGES */
	public byte[] receiveNextMessage() throws IOException ; 
}
