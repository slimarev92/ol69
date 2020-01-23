package il.co.ilrd.ws14;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class PingTCP {
	private final String message = "ping";
	private final int port = 7777;
	private final Socket socket;
	private final int DELAY = 2000;
	private boolean isRunning = true;
	private final String tty = "/dev/pts/7";
	
	public PingTCP() throws UnknownHostException, IOException {		
		System.setOut(new PrintStream(new File(tty)));
		socket = new Socket(InetAddress.getLocalHost(), port);
	}
	
	public void run() {
		System.out.println("Starting  " + message);
		
		try(PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);
			BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {
			
			while(isRunning) {
				writer.println(message);
				System.out.println(reader.readLine());

				try {
					Thread.sleep(DELAY);
				} catch (InterruptedException e) {
					continue;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		try {
			new PingTCP().run();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
