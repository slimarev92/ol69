package il.co.ilrd.ws14;

import java.io.IOException;
import java.io.PrintStream;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Set;

public class PingSelector {
	private final int PORT = 7777;
	private final String MESSAGE = "pong";
	private final Selector selector;
	private final ServerSocketChannel socket;
	private volatile boolean isRunning = true;
	private final ByteBuffer readBuffer = ByteBuffer.allocate(128);
	private final ByteBuffer bufferedMessage = ByteBuffer.wrap(MESSAGE.getBytes());
	private final String tty = "/dev/pts/7";
	
	public PingSelector() throws IOException {
		selector = Selector.open();
		socket = ServerSocketChannel.open();
		socket.bind(new InetSocketAddress(PORT));
		
		socket.configureBlocking(false);
		socket.register(selector, SelectionKey.OP_ACCEPT);
		
		System.setOut(new PrintStream(tty));
	}
	
	public void run() throws IOException {
		boolean shouldRead = true;
		boolean readable = false;
		boolean writable = false;
		
		System.out.println("Starting selector ping (server)");
		
		while(isRunning) {
			readable = false;
			writable = false;
				
			try {
				selector.select();
			} catch (IOException e) { e.printStackTrace(); }
			
			Set<SelectionKey> keys = selector.selectedKeys();
			
			for(SelectionKey currKey : keys) {				
				if(currKey.isAcceptable()) {
					ServerSocketChannel serverSocket = (ServerSocketChannel)currKey.channel();
					SocketChannel newClient = serverSocket.accept();
					newClient.configureBlocking(false);
					newClient.register(selector, SelectionKey.OP_READ | SelectionKey.OP_WRITE);
				} 
				
				if(currKey.isReadable()) {
					readable = true;
				} 
				
				if(currKey.isWritable()) {
					writable = true;
				}
				
				keys.remove(currKey);		
				
				if(readable && shouldRead) {
					SocketChannel channel = (SocketChannel)currKey.channel();
					readBuffer.clear();
					channel.read(readBuffer);
					System.out.println(new String(readBuffer.array()));
					shouldRead = false;
					
					try {
						Thread.sleep(1000);
					} catch(InterruptedException e) {}
	
				} else if(writable && !shouldRead) {
					SocketChannel channel = (SocketChannel)currKey.channel();	
					channel.write(bufferedMessage);
					bufferedMessage.rewind();
					shouldRead = true;
				}
			}
		}
	}
	
	public static void main(String[] args) {
		try {
			new PingSelector().run();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
