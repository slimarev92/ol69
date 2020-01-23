package il.co.ilrd.ws14;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.Arrays;

public class PongUDP {
	private final String message = "pong";
	private final String tty = "/dev/pts/7";
	private final DatagramSocket socket;
	private final int port = 7777;
	private boolean isRunning = true;
	private final byte[] buffer = new byte[128];
	private final int DELAY = 3000;
	
	public PongUDP() throws FileNotFoundException, SocketException  {
		System.setOut(new PrintStream(new File(tty)));
		socket = new DatagramSocket(port);
	}
	
	public void run() throws IOException {
		
		System.out.println("Starting UDP pong");
		
		while(isRunning) {
			Arrays.fill(buffer, (byte)0);
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			
			socket.receive(packet);
			
			System.out.println(new String(packet.getData()));
				
			try {
				Thread.sleep(DELAY);
			} catch(InterruptedException e) { continue; }
			
			packet = new DatagramPacket(message.getBytes(), message.length(), packet.getAddress(), packet.getPort());
			socket.send(packet);
		}
	}
	
	public static void main(String[] args) {
		try {
			new PongUDP().run();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
