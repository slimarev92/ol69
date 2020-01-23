package il.co.ilrd.chat;

public interface LogicManager {
	public void handleNewClient(OutgoingMessageHandler commObj);
	public void handleMessage(OutgoingMessageHandler commObj, String message);
	public void handleDisconnect(OutgoingMessageHandler commObj);
}
