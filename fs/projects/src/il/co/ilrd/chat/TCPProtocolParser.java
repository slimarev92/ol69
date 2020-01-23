package il.co.ilrd.chat;

public class TCPProtocolParser implements ChatProtocolParser {
	//private static final String messageBookend = new String(new byte[] {0X1D}); /* 1D = GROUP SEPRATOR */
	private static final String messageBookend = "#@";
	
	@Override
	public String wrapMessage(String message) {
		StringBuilder protocolMessageBuilder = new StringBuilder();
		protocolMessageBuilder.append(messageBookend);
		protocolMessageBuilder.append(message);
		protocolMessageBuilder.append(messageBookend);
			
		return protocolMessageBuilder.toString();
	}

	@Override
	public String getNextMessage(StringBuilder rawData) {
		clearCorruptData(rawData);
		
		int firstOccurance = rawData.indexOf(messageBookend);
		String retVal = null;
		
		int secondOccurance = rawData.indexOf(messageBookend, firstOccurance + messageBookend.length());

		if (2 <= secondOccurance) {
			retVal = rawData.substring(0, secondOccurance + messageBookend.length()).replaceAll(messageBookend, "");
			rawData.delete(0, secondOccurance + messageBookend.length());
		}
		
		return retVal;
	}
	
	private void clearCorruptData(StringBuilder rawData) {
		int firstOccurance = rawData.indexOf(messageBookend);
		
		if(0 < firstOccurance) { rawData.delete(0, firstOccurance); }
	}
}