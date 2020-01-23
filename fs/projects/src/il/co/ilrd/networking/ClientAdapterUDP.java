package il.co.ilrd.networking;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.concurrent.TimeoutException;

public class ClientAdapterUDP implements ClientAdapter {
	private final DatagramSocket socket;
	private final InetAddress address;
	private final int port;

	public ClientAdapterUDP(String address, int port) throws UnknownHostException, SocketException {
		this.address = InetAddress.getByName(address);
		this.port = port;
		
		socket = new DatagramSocket();
	}

	@Override
	public void send(byte[] data) throws IOException {
		socket.send(new DatagramPacket(data, data.length, address, port));
	}
	
	@Override
	public byte[] receive(int size, int timeout) throws IOException, TimeoutException {
		byte[] receiveBuffer = new byte[size];
		DatagramPacket packet = new DatagramPacket(receiveBuffer, receiveBuffer.length);
		
		socket.setSoTimeout(timeout);
		
		try {
			socket.receive(packet);
		} catch (SocketTimeoutException e) { throw new TimeoutException(); }

		return packet.getData(); 
	}

	@Override
	public void stop() {
		socket.close();
	}
}
