package il.co.ilrd.playground;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Set;


public class Selectors {
	private final int PORT = 7777;
	private final String MESSAGE = "pong";
	private final Selector selector;
	private final ServerSocketChannel socket;
	private volatile boolean isRunning = true;
	private final ByteBuffer readBuffer = ByteBuffer.allocate(128);
	private final ByteBuffer bufferedMessage = ByteBuffer.wrap(MESSAGE.getBytes());
	private final String tty = "/dev/pts/7";
	
	public Selectors() throws IOException {
		selector = Selector.open();
		socket = ServerSocketChannel.open();
		socket.bind(new InetSocketAddress(PORT));
		
		socket.configureBlocking(false);
		socket.register(selector, SelectionKey.OP_ACCEPT);
	}
	
	public void run() throws IOException {
		while(isRunning) {
			try {
				selector.select();
			} catch (IOException e) { e.printStackTrace(); }
			
			Set<SelectionKey> keys = selector.selectedKeys();
			
			for(SelectionKey currKey : keys) {				
				if(currKey.isAcceptable()) {
					ServerSocketChannel serverSocket = (ServerSocketChannel)currKey.channel();
					SocketChannel newClient = serverSocket.accept();
					newClient.register(selector, SelectionKey.OP_READ | SelectionKey.OP_WRITE);
				} else if(currKey.isReadable()) {
					SocketChannel channel = (SocketChannel)currKey.channel();
					readBuffer.clear();
					channel.read(readBuffer);
					System.out.println(new String(readBuffer.array()));
				} else if(currKey.isWritable()) {
					SocketChannel channel = (SocketChannel)currKey.channel();	
					channel.write(bufferedMessage);
				}
			}
		}
	}
}
