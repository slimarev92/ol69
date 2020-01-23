package il.co.ilrd.ws14;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class EchoServerUDP {
	public static void main(String[] args) {
		final int port = 7777;
		boolean isRunning = true;
		DatagramSocket socket;
		byte[] buffer = new byte[64];
		
		System.out.println("Starting echo server...");
		
		try {
			socket = new DatagramSocket(port);
		} catch(SocketException e) { return; }
		
		while(isRunning) {
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			try {
				socket.receive(packet);
			} catch (IOException e) { return; }
			
			packet = new DatagramPacket(packet.getData(), packet.getData().length, packet.getAddress(), packet.getPort());
			
			try {
				socket.send(packet);
			} catch (IOException e) { return; }
		}
		
		socket.close();
	}
	
	
}
