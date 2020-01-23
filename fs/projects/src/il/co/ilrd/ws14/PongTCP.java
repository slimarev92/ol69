package il.co.ilrd.ws14;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

public class PongTCP {
	private final String message = "pong";
	private final int port = 7777;
	private final ServerSocket socket;
	private final int DELAY = 2000;
	private boolean isRunning = true;
	private final String tty = "/dev/pts/7";
	private Socket client;
	
	public PongTCP() throws UnknownHostException, IOException {	
		System.setOut(new PrintStream(new File(tty)));
		socket = new ServerSocket(port);
	}
	
	public void run() throws IOException {
		System.out.println("Starting  " + message);
		
		client = socket.accept();
		
		System.out.println("accepted client");
		try (PrintWriter writer = new PrintWriter(client.getOutputStream(), true);
			BufferedReader reader = new BufferedReader(new InputStreamReader(client.getInputStream()))) {
			
			while (isRunning) {
				System.out.println(reader.readLine());
				writer.println(message);

				try {
					Thread.sleep(DELAY);
				} catch (InterruptedException e) {
					continue;
				}
			}
		}
	}
	
	public static void main(String[] args) {
		try {
			new PongTCP().run();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
