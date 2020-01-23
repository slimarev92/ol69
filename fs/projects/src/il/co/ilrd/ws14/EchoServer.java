package il.co.ilrd.ws14;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {
    public static void main(String[] args) throws IOException {
    	int port = 7777;
    	
    	System.out.println("Starting echo server...");
    	
    	try(ServerSocket listenSocket = new ServerSocket(port);
    		Socket echoClient = listenSocket.accept();
    		BufferedReader clientReader = new BufferedReader(new InputStreamReader(echoClient.getInputStream()));
    		PrintWriter clientWriter = new PrintWriter(echoClient.getOutputStream(), true)) {
    		
    		String line = null;
    		
    		while(null != (line = clientReader.readLine())) {
    			clientWriter.println(line);
    		}
    	} catch(IOException e) {
    		e.printStackTrace();
    	} 
    	
    	System.out.println("Stopping echo server...");
    }
}