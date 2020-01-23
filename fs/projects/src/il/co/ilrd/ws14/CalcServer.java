package il.co.ilrd.ws14;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class CalcServer {
	private final int port = 7777; 
	private CalcProtocol protocol = new CalcProtocol();
	
	public void startServer() {
		try(ServerSocket serverSocket = new ServerSocket(port);
			Socket clientSocket = serverSocket.accept();
				PrintWriter clientWriter = new PrintWriter(clientSocket.getOutputStream(), true);
				BufferedReader clientReader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {
			
			String clientInput = null;
			
			clientWriter.println("STARTING CALC SERVER, ENTER OPERAND");
			
			while(null != (clientInput = clientReader.readLine())) {
				String answer = protocol.proccessInput(clientInput);
				clientWriter.println(answer);
			}	
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		System.out.println("STARTING CALC SERVER");
		new CalcServer().startServer();
		System.out.println("STOPPING CALC SERVER");
	}
}
