package il.co.ilrd.ws14;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class EchoClient {
	private final int port = 7777;

	public void startEchoes() {
		try(Socket echoSocket = new Socket(InetAddress.getLocalHost(), port);
			PrintWriter socketOutput = new PrintWriter(echoSocket.getOutputStream(), true);	
			BufferedReader echoReader = new BufferedReader(new InputStreamReader(echoSocket.getInputStream()));
			Scanner inputScanner = new Scanner(System.in)) {
						
			System.out.println(echoReader.readLine());
			String echo = null;
					
			while(null != (echo = inputScanner.nextLine())) {
				socketOutput.println(echo);
				System.out.println(echoReader.readLine());
			}	
		} catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) throws UnknownHostException {	
		System.out.println("starting echo client");
		
		EchoClient echoClient = new EchoClient();
		echoClient.startEchoes();
	}
}
