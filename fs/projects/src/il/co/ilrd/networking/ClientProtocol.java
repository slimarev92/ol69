package il.co.ilrd.networking;

import java.io.IOException;
import java.net.ProtocolException;

public interface ClientProtocol {
	public void sendMessage(byte[] data) throws IOException, ProtocolException;
}
