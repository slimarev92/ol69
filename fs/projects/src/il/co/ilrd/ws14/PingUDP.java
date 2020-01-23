package il.co.ilrd.ws14;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.Arrays;

public class PingUDP {
	private final String message = "ping";
	private final String tty = "/dev/pts/7";
	private final DatagramSocket socket;
	private final int port = 7777;
	private boolean isRunning = true;
	private final byte[] buffer = new byte[128];
	private final int DELAY = 3000;
	
	public PingUDP() throws FileNotFoundException, SocketException  {
		System.setOut(new PrintStream(new File(tty)));
		socket = new DatagramSocket();
	}
	
	public void run() throws IOException {
		
		System.out.println("Starting UDP ping");
		
		while(isRunning) {
			DatagramPacket packet = new DatagramPacket(message.getBytes(), message.length(), InetAddress.getLocalHost(), port);
			
			socket.send(packet);
			
			Arrays.fill(buffer, (byte)0);
			packet = new DatagramPacket(buffer, buffer.length);
			
			socket.receive(packet);
			
			try {
				Thread.sleep(DELAY);
			} catch(InterruptedException e) { continue; }
			
			System.out.println(new String(packet.getData()));
			
		
		}
	}
	
	public static void main(String[] args) {
		try {
			new PingUDP().run();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
