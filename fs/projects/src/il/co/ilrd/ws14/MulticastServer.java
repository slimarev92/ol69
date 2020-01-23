package il.co.ilrd.ws14;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class MulticastServer {
	private final int port = 7777;
	private final InetAddress group;
	private final DatagramSocket socket;
	private boolean isRunning = true;
	private final String[] jokes = { "What's Blonde and dead in a closet? The Hide and Seek Champion from 1995.",
			                         "Moses had the first tablet that could connect to the cloud.",
			                         "Most people are shocked when they find out how incompetent I am as an electrician.",
			                         "eBay is so useless. I tried to look up lighters and all they had was 13,749 matches.",
			                         "I can't believe I got fired from the calendar factory. All I did was take a day off."
	                               };
	
	
	public MulticastServer() throws IOException {
		group = InetAddress.getByName("225.23.23.23");
		socket = new DatagramSocket();
	}
	
	public void startServer() throws IOException {
		while(isRunning) {
			int index = ((int)(Math.random() * 10)) % jokes.length;
			byte[] jokeAsBytes = jokes[index].getBytes();
			
			DatagramPacket packet = new DatagramPacket(jokeAsBytes, jokeAsBytes.length, group, port);
					
			socket.send(packet);
			
			try {
				Thread.sleep(5000);
			} catch (InterruptedException e) { continue; }
		}
	}
	
	public void stopServer() {
		socket.close();
	}

	public static void main(String[] args) {
		MulticastServer server;
		
		System.out.println("starting joke server...");
		
		try {
			server = new MulticastServer();
			server.startServer();
		} catch (IOException e) {
			e.printStackTrace();
			return;
		}
	}
}
