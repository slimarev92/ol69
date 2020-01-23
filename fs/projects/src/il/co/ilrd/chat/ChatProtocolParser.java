package il.co.ilrd.chat;

public interface ChatProtocolParser {
	public String wrapMessage(String message);
	public String getNextMessage(StringBuilder rawData);

}
