package il.co.ilrd.logmonitor;

import java.io.IOException;
import java.net.ProtocolException;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.concurrent.TimeoutException;

import il.co.ilrd.networking.ClientAdapter;
import il.co.ilrd.networking.ClientAdapterUDP;
import il.co.ilrd.networking.ClientProtocol;

public class ClientLogProtocol implements ClientProtocol {
	private static final String MESSAGE_BOOKEND = "@#"; 
	private static final int NUM_SEND_TRIES = 3;
	
	private static final int TIMEOUT = 50;
	private final ClientAdapter adapter;
	private long messageCount = 0;
	
	public ClientLogProtocol(String address, int port) throws IOException {
		adapter = new ClientAdapterUDP(address, port);
	}

	@Override
	public void sendMessage(byte[] data) throws IOException, ProtocolException {
		boolean messageReceived = false;
		
		byte[] wrappedMessage = wrapInProtocol(data);
				
		for(int i = 0; NUM_SEND_TRIES > i && !messageReceived; ++i) {	
			adapter.send(wrappedMessage);
			
			System.out.println("sent message |" + wrappedMessage + "|");
		
			byte[] ack = null;
			
			try {
				ack = adapter.receive(12, TIMEOUT);					
			} catch (TimeoutException e) {
				System.out.println("timeout");
				continue;
			}
			
			if(getMessageID(ack) == messageCount) {
				messageReceived = true;
			} else {
				System.out.println("Bad ID");
				System.out.println(Arrays.toString(ack));
			}
		}
		
		++messageCount;
		
		if(!messageReceived) { throw new ProtocolException(); }
	}
	
	private static long getMessageID(byte[] message) {
		ByteBuffer messageBytes = ByteBuffer.wrap(message);
		
		if (12 > message.length             ||
			messageBytes.get(0)  != '@'     || 
			messageBytes.get(1)  != '#'     || 
			messageBytes.get(10) != '@'     || 
			messageBytes.get(11) != '#') {
			
			return -1;
		}
	
		return messageBytes.getLong(2);
	}
	
	private byte[] wrapInProtocol(byte[] data) {
		ByteBuffer wrappedMessage = ByteBuffer.allocate(data.length + 12);
			
		wrappedMessage.put(MESSAGE_BOOKEND.getBytes()); /* #@ */
		wrappedMessage.putLong(messageCount); /* ID */
		wrappedMessage.put(data); /* ACTUAL DATA */
		wrappedMessage.put(MESSAGE_BOOKEND.getBytes()); /* #@ */		
		
		return wrappedMessage.array();
	}
}
