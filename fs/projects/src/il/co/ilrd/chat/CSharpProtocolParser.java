package il.co.ilrd.chat;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class CSharpProtocolParser implements ChatProtocolParser {
	private static final byte messageBookend = 0x1D;
	/* 1D = GROUP SEPRATOR */
	private static final String messageBookendStr = new String(new byte[] {messageBookend}); 
	
	
	@Override
	public String wrapMessage(String message) {
		ByteBuffer buffer = ByteBuffer.allocate(message.length() + 8);
		
		buffer.order(ByteOrder.LITTLE_ENDIAN);
		
		buffer.put(messageBookend);
		
		buffer.putInt(message.length() + 2);
		
		buffer.put(messageBookend);
		
		
		buffer.put(messageBookend);
		
		buffer.put(message.getBytes(java.nio.charset.StandardCharsets.US_ASCII));
		
		buffer.put(messageBookend);
		
		return new String(buffer.array());
	}

	@Override
	public String getNextMessage(StringBuilder rawData) {
		clearCorruptData(rawData);
		
		int firstOccurance = rawData.indexOf(messageBookendStr);
		String retVal = null;
		
		int secondOccurance = rawData.indexOf(messageBookendStr, firstOccurance + messageBookendStr.length());

		if (2 <= secondOccurance) {
			retVal = rawData.substring(0, secondOccurance + messageBookendStr.length()).replaceAll(messageBookendStr, "");
			rawData.delete(0, secondOccurance + messageBookendStr.length());
		}
		
		return retVal;
	}
	
	private void clearCorruptData(StringBuilder rawData) {
		int firstOccurance = rawData.indexOf(messageBookendStr);
		
		if(0 < firstOccurance) { rawData.delete(0, firstOccurance); }
	}
}