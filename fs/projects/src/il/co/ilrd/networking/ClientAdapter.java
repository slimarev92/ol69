package il.co.ilrd.networking;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public interface ClientAdapter {
	public void send(byte[] msg) throws IOException; /* SENDS MESSAGE TO SERVER */
	public byte[] receive(int size, int timeout) throws IOException, TimeoutException; /* RECEIVE IS TIMED, NOT BLOCKING FOREVER */
	public void stop();
}
