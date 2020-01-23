package il.co.ilrd.ws14;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Scanner;

public class EchoClientUDP {
	public static void main(String[] args) {
		final int port = 7777;
		InetAddress host = null;
		DatagramSocket socket = null;
		byte[] buffer = new byte[64];
		Scanner inputScanner = new Scanner(System.in);
		boolean isRunning = true;
		
		System.out.println("Starting echo client...");
		
		try {
			host = InetAddress.getLocalHost();
		} catch (UnknownHostException e) { }
		
		try {
			socket = new DatagramSocket();
		} catch (SocketException e) { }
		
		while(isRunning) {
			
			buffer = inputScanner.nextLine().getBytes();
			
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, host, port);
			
			try {
				socket.send(packet);
				socket.receive(packet);
				
				System.out.println(new String(packet.getData(), 0 ,packet.getLength()));
	
			} catch (IOException e) { }
		}	
		
		socket.close();
		inputScanner.close();
	}
}
