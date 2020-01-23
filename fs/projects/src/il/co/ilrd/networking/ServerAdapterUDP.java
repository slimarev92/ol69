package il.co.ilrd.networking;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Arrays;

public class ServerAdapterUDP implements ServerAdapter {
	private final DatagramSocket serverSocket;
	
	public ServerAdapterUDP(String address, int port) throws UnknownHostException, SocketException {
		serverSocket = new DatagramSocket(port, InetAddress.getByName(address));
	}
	
	@Override
	public Message receive(int size) throws IOException {
		DatagramPacket packet = new DatagramPacket(new byte[size], size);
			
		serverSocket.receive(packet);
		
		return new MessageImp(Arrays.copyOf(packet.getData(), packet.getLength()), packet.getAddress(), packet.getPort());
	}
	
	private class MessageImp implements Message {
		private final InetAddress address;
		private final byte[] data;
		private final int port;
		
		public MessageImp(byte[] data, InetAddress address, int port) {
			this.data = data;
			this.address = address;
			this.port = port;
		}
		
		@Override
		public void reply(byte[] data) throws IOException {
			serverSocket.send(new DatagramPacket(data, data.length, address, port));
		}

		@Override
		public byte[] getData() {
			return data;
		}
	}

	@Override
	public void close() {
		serverSocket.close();
	}
}
