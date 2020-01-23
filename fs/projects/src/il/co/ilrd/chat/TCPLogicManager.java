package il.co.ilrd.chat;

import java.util.HashMap;

public class TCPLogicManager implements LogicManager {
	private final HashMap<OutgoingMessageHandler, UserInfo> users = new HashMap<>();
	
	@Override
	public void handleNewClient(OutgoingMessageHandler commObj) {
		users.put(commObj, new UserInfo());
		
		sendNameMessage(commObj);
	}

	private void sendNameMessage(OutgoingMessageHandler commObj) {
		commObj.handleMessage("Please enter your name");
	}
	
	@Override
	public void handleMessage(OutgoingMessageHandler senderCommObj, String message) {
		UserInfo sender = users.get(senderCommObj);
		String finalMessage = sender.getName() + ": " + message;
		
		if(!sender.isConnected()) { /* user left */
			users.forEach((OutgoingMessageHandler commObject, UserInfo userInfo) -> {
				if (commObject != senderCommObj) {
					commObject.handleMessage(finalMessage);
				}
			});
		} else if(sender.isConnected() && sender.isNameSet()) { /* regular message */
			users.forEach((OutgoingMessageHandler commObject, UserInfo userInfo) -> commObject.handleMessage(finalMessage));
		} else { /* user logged on message */
			sender.setName(message);
			
			String loggonMessage = sender.getName() + " has logged on";	
			users.forEach((OutgoingMessageHandler commObject, UserInfo userInfo) -> commObject.handleMessage(loggonMessage));
		}
	}
	
	@Override
	public void handleDisconnect(OutgoingMessageHandler commObj) {	
		UserInfo sender = users.get(commObj);
		
		sender.disconnected();
		handleMessage(commObj, "* left the chat *");
		users.remove(commObj);
	}

	private static class UserInfo {
		private String name = "new user";
		private boolean nameSet = false;
		private boolean connected = true;
		
		public void setName(String name) {
			this.name = name;
			nameSet = true;
		}
		
		public boolean isNameSet() {
			return nameSet;
		}
		
		public String getName() {
			return name;
		}
		
		public boolean isConnected() {
			return connected;
		}
		
		public void disconnected() {
			connected = false;
		}
	}
}