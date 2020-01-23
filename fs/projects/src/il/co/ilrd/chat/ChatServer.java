package il.co.ilrd.chat;

import java.io.IOException;
import java.io.PrintStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.HashMap;
import java.util.Iterator;

public class ChatServer {
	private final int READ_BUFFER_CAPACITY = 256;
	private final int DONE_READING = 0;
	private final int EOS = -1;
	
	private final HashMap<OutgoingMessageHandler, StringBuilder> clientsRawData = new HashMap<>();
	private final ByteBuffer readBuffer = ByteBuffer.allocate(READ_BUFFER_CAPACITY);
	private final LogicManager logicManager;
	private boolean serverRunning = true;
	private final ChatProtocolParser protocol;
    private final Selector selector;
    
    public ChatServer(InetSocketAddress address, LogicManager logicManager, ChatProtocolParser protocol) throws IOException {
    	ServerSocketChannel acceptSocket = ServerSocketChannel.open();
    	selector = Selector.open();
               
        acceptSocket.bind(address);
        acceptSocket.configureBlocking(false);
        acceptSocket.register(selector, SelectionKey.OP_ACCEPT);
        
        this.logicManager = logicManager;
        this.protocol = protocol;
    }
    
    public ChatServer(String logFile, InetSocketAddress serverAddress, LogicManager logicManager, ChatProtocolParser protocol) throws IOException {
    	this(serverAddress, logicManager, protocol);
        
        System.setOut(new PrintStream(logFile));
        System.setErr(new PrintStream(logFile)); 
    }
         
    public void startServer() {
        System.out.println("Starting chat server...");
        
        while(serverRunning) {
            try {
                selector.select();
            } catch (IOException e) {
            	System.err.println("IOException while using selector.");
                continue;
            }
            
            Iterator<SelectionKey> keysIter = selector.selectedKeys().iterator();
            
            while(keysIter.hasNext()) {
                SelectionKey currKey = keysIter.next();
                
                if(currKey.isAcceptable()) {
                    try {
                        ServerSocketChannel acceptSocket = (ServerSocketChannel)currKey.channel();
                        addClient(acceptSocket.accept());   
					} catch(IOException e) {
						System.err.println("IOException while accepting new client");
					}
                } else if(currKey.isReadable()) {
                    try {
                    	handleReadableClient(currKey);
                    } catch(IOException e) {
                    	disconnectClient(currKey);
                    	System.err.println("Client left the chat due to an error: " + currKey.channel());
                        System.out.println("Client left the chat due to an error: " + currKey.channel());
                    }
                }
                                
                keysIter.remove();
            }
        }
    }
       
    private void handleReadableClient(SelectionKey key) throws IOException {
    	 SocketChannel clientSocket = (SocketChannel)key.channel();
    	 MessageHandlerImpl sender = (MessageHandlerImpl)key.attachment();
         boolean clientIsUp = readClientData(sender, clientSocket);
         
         if(clientIsUp) {
        	 handleNewMessages(sender); 	   
         } else {
        	 disconnectClient(key);
      	   
        	 System.out.println("Client left the chat: " + key.channel());
         }
    }
    
    private void addClient(SocketChannel newClient) throws IOException {    
    	MessageHandlerImpl clientCommObj = new MessageHandlerImpl(newClient, protocol); 
        
        newClient.configureBlocking(false);
        SelectionKey clientKey = newClient.register(selector, SelectionKey.OP_READ);        
        clientKey.attach(clientCommObj);
        clientsRawData.put(clientCommObj, new StringBuilder(READ_BUFFER_CAPACITY));
        
        logicManager.handleNewClient(clientCommObj);
        System.out.println("Added new client " + newClient);
    }
    
    private void handleNewMessages(MessageHandlerImpl sender) {
		StringBuilder clientRawData = clientsRawData.get(sender);
		String nextMessage = null;

		while (null != (nextMessage = protocol.getNextMessage(clientRawData))) {
			logicManager.handleMessage((OutgoingMessageHandler) sender, nextMessage);
		}
	}
    
    private boolean readClientData(MessageHandlerImpl client, SocketChannel clientSocket) throws IOException {
    	StringBuilder clientRawData = clientsRawData.get(client);
        int bytesRead = 0;
        
        while(DONE_READING != (bytesRead = clientSocket.read(readBuffer))) {
            if(EOS == bytesRead) { return false; }
            	clientRawData.append(new String(readBuffer.array(), 0, bytesRead));    
            	readBuffer.rewind();
        }
        
        return true;
    }
    
    private void disconnectClient(SelectionKey clientKey) {
    	clientsRawData.remove(clientKey.attachment());
    	logicManager.handleDisconnect((OutgoingMessageHandler)clientKey.attachment());
    	clientKey.cancel();
    }
     
    public void stopServer() {
        serverRunning = false;
        
        try {
            selector.close();
        } catch (IOException e) {
            System.err.println("IOException while shutting down server");
        }
        
        System.out.println("Shutting server down.");
    }
    
    private class MessageHandlerImpl implements OutgoingMessageHandler {
        private final SocketChannel clientSocket;

        public MessageHandlerImpl(SocketChannel clientSocket, ChatProtocolParser protocol) {
            this.clientSocket = clientSocket;
        }
        
        @Override
        public void handleMessage(String message) {     
        	ByteBuffer writeBuffer = ByteBuffer.wrap(protocol.wrapMessage(message).getBytes());
       	
            try {
                clientSocket.write(writeBuffer);
                System.out.println("Wrote message |" + new String(writeBuffer.array()) + "|");
            } catch (IOException e) {
                System.err.println("Couldn't write message to user " + clientSocket.toString());
            }
        }
    }
    
    public static void main(String[] args) throws InterruptedException {
        try {      	
			new ChatServer(new InetSocketAddress(InetAddress.getByName("localhost"), 7777), new TCPLogicManager(),
					new TCPProtocolParser()).startServer();
        } catch(IOException e) { e.printStackTrace(); }
    }
}