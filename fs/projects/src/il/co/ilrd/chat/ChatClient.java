package il.co.ilrd.chat;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import il.co.ilrd.concurrency.WaitableQueue;
import il.co.ilrd.concurrency.WaitableQueueSemaphore;

public class ChatClient {
	private final int BUFFER_CAPACITY = 256;	
	private final int EOS = -1;
	
	private final WaitableQueue<String> outgoingMessageQueue = new WaitableQueueSemaphore<>();
	private final WaitableQueue<String> incomingMessageQueue = new WaitableQueueSemaphore<>();
	private MessageReader messageReader;
	private final ChatProtocolParser protocol;
	private OutputStreamWriter writer;
	private Socket serverSocket;
	private Reader reader;
	private ChatUI UI;
	
	public ChatClient(ChatProtocolParser protocol) {
		this.protocol = protocol;
	}
	
	public void startClient() throws UnknownHostException, IOException, InterruptedException {
		boolean gotIPPort = false;
		UI = new ChatUI(outgoingMessageQueue, incomingMessageQueue);

		UI.displayMessage("Enter IP and port");
		
		while(!gotIPPort) {
			
			if(!UI.isOpen()) { return; }
			
			String IPPort[] = outgoingMessageQueue.dequeue().split(" ");
			
			if(IPPort.length < 2) {
				UI.displayMessage("Connection Error. Enter IP and port again"); 
				continue;
			}
			
			try {
				serverSocket = new Socket(InetAddress.getByName(IPPort[0]), Integer.parseInt(IPPort[1]));	
			} catch(Exception e) { UI.displayMessage("Connection Error. Enter IP and port again"); continue;} 
			
			gotIPPort = true;
		}

		writer = new OutputStreamWriter(serverSocket.getOutputStream());
		reader = new InputStreamReader(serverSocket.getInputStream());
		
		messageReader = new MessageReader();
		messageReader.start();
		
		try {
			mainLoop();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private void mainLoop() throws InterruptedException, IOException {
		while(UI.isOpen()) {
			String message = outgoingMessageQueue.dequeue();
			
			if(!message.trim().isEmpty()) { 
				
				String wrappedMessage = protocol.wrapMessage(message);
				
				writer.write(wrappedMessage);
				System.out.println("Message sent (len) " + wrappedMessage.length());
				writer.flush();
			} 
		}
			
		messageReader.stopRunning();
		serverSocket.close();
		writer.close();
	}
			
	private class MessageReader extends Thread {	
		private volatile boolean isRunning = true;
		
		public void stopRunning() {
			isRunning = false;
		}
		
		@Override
		public void run() {
			try {
				StringBuilder readData = new StringBuilder();
				char[] buffer = new char[BUFFER_CAPACITY];
				
				while(isRunning) {
					int bytesRead = reader.read(buffer);
					
					if(EOS == bytesRead) {
						UI.displayMessage("Server is down.");
						isRunning = false;
						break;
					}
					
					readData.append(buffer, 0, bytesRead);

					boolean moreMessages = true;
					
					while(moreMessages) {			        	
			        	String parsedMessage = protocol.getNextMessage(readData);
			        	
			        	if(null != parsedMessage) {
			        		UI.displayMessage(parsedMessage);
			        	} else {
			        		moreMessages = false;       		
			        	}
					}			
				}
			} catch (IOException e) {
				System.out.println(e.toString());
				UI.displayMessage("Server is down");
				isRunning = false;
			}
		}
	}
	
	public static void main(String[] args) {
		try {
			new ChatClient(new CSharpProtocolParser()).startClient();	
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}