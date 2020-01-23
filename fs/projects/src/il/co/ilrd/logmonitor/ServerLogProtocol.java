package il.co.ilrd.logmonitor;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import il.co.ilrd.networking.ServerAdapterUDP;
import il.co.ilrd.networking.ServerProtocol;
import il.co.ilrd.networking.ServerAdapter;
import il.co.ilrd.networking.Message;

public class ServerLogProtocol implements ServerProtocol {
	private final Set<Long> lastMessages = new HashSet<>();
	private final ServerAdapter adapter;
	
	private static final String MESSAGE_BOOKEND = "@#"; 
	private static final int MESSAGE_SIZE = 512;
	
	public ServerLogProtocol(String address, int port) throws IOException {
		adapter = new ServerAdapterUDP(address, port);
	}

	@Override
	public byte[] receiveNextMessage() throws IOException {	
		byte[] retVal = null;
		
		while(null == retVal) {		
			Message currMessage = adapter.receive(MESSAGE_SIZE);
				
			long ID = getMessageID(currMessage.getData());
			
			currMessage.reply(wrapInProtocol(ID));
			
			if (-1 != ID && !lastMessages.contains(ID)) {
				lastMessages.add(ID);
				byte[] extractedMessage = extractMessage(currMessage.getData());

				retVal = extractedMessage;	
			}
		}

		return retVal;
	}
	
	private static long getMessageID(byte[] message) {
		ByteBuffer messageBytes = ByteBuffer.wrap(message);
		
		if (12 > message.length                         ||
			messageBytes.get(0) != '@'                  || 
			messageBytes.get(1) != '#'                  || 
			messageBytes.get(message.length - 2) != '@' || 
			messageBytes.get(message.length - 1) != '#') {
						
			return -1;
		}
		
		return messageBytes.getLong(2);
	}
	
	private static byte[] extractMessage(byte[] message) {		
		return Arrays.copyOfRange(message, 10, message.length - 2);
	}
	
	private byte[] wrapInProtocol(long ID) {
		ByteBuffer wrappedMessage = ByteBuffer.allocate(12);
		
		wrappedMessage.put(MESSAGE_BOOKEND.getBytes()); /* #@ */
		wrappedMessage.putLong(ID); /* ID */
		wrappedMessage.put(MESSAGE_BOOKEND.getBytes()); /* #@ */
			
		return wrappedMessage.array();
	}	
}
