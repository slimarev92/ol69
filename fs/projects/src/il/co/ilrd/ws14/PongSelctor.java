package il.co.ilrd.ws14;

import java.io.IOException;
import java.io.PrintStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Set;

public class PongSelctor {
	private final int PORT = 7777;
	private final String MESSAGE = "ping";
	private final Selector selector;
	private final SocketChannel socket;
	private volatile boolean isRunning = true;
	private final ByteBuffer readBuffer = ByteBuffer.allocate(128);
	private final ByteBuffer bufferedMessage = ByteBuffer.wrap(MESSAGE.getBytes());
	private final String tty = "/dev/pts/7";
	
	public PongSelctor() throws IOException {
		selector = Selector.open();
		socket = SocketChannel.open();

		System.setOut(new PrintStream(tty));
	}
	
	public void run() throws IOException {
		boolean shouldRead = false;
		boolean readable = false;
		boolean writable = false;
		
		System.out.println("Starting selector pong (client)");
		
		socket.connect(new InetSocketAddress(InetAddress.getLocalHost(), PORT));
		socket.configureBlocking(false);
		socket.register(selector, SelectionKey.OP_READ | SelectionKey.OP_WRITE);
		
		while(isRunning) {
			readable = false;
			writable = false;
				
			try {
				selector.select();
			} catch (IOException e) { e.printStackTrace(); }
			
			Set<SelectionKey> keys = selector.selectedKeys();
			
			for(SelectionKey currKey : keys) {			
				
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
			new PongSelctor().run();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
