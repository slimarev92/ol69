package il.co.ilrd.ws14;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.util.Arrays;

public class MulticastClient {
	private final int port = 7777;
	private final InetAddress group;
	private final MulticastSocket groupSocket = new MulticastSocket(port);
	private boolean isRunning = true;
	private final byte[] buffer = new byte[128];
	
	public MulticastClient() throws IOException {
		group = InetAddress.getByName("225.23.23.23");
		groupSocket.joinGroup(group);
	}
	
	public void startClient() throws IOException {
		while(isRunning) {
			Arrays.fill(buffer, (byte)0);
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			groupSocket.receive(packet);
			
			System.out.println(new String(packet.getData()));
			
			try {
				Thread.sleep(5000);				
			} catch(InterruptedException e) { continue; }
		}
	}
	
	public static void main(String[] args) {
		System.out.println("starting joke client...");
		
		try {
			new MulticastClient().startClient();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
