package il.co.ilrd.logmonitor;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.function.Consumer;

import il.co.ilrd.crud.CRUD;
import il.co.ilrd.crud.CRUDFile;
import il.co.ilrd.networking.ServerProtocol;

public class MonitorServer {
	private final ServerProtocol protocol;
	private final List<Consumer<byte[]>> handlers = new LinkedList<>();
	
	public MonitorServer(ServerProtocol protocol) throws IOException {
		this.protocol = protocol;	
	}

	public void start() {
		while (true) {
			try {
				byte[] newMessage = protocol.receiveNextMessage();
				handlers.forEach(consumer -> consumer.accept(newMessage));

			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void registerNewMessage(Consumer<byte[]> onNewMessage) {
		handlers.add(onNewMessage);
	}
	
	public static void main(String[] args) {
		try (CRUD<Integer, String> file = new CRUDFile("/home/sasha/docs/to.txt")) {
			MonitorServer server = new MonitorServer(new ServerLogProtocol("localhost", 7777));
			server.registerNewMessage(event -> file.create(new String(event)));
			server.registerNewMessage(event -> System.out.println("event : |" + new String(event) +"|"));

			server.start();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
