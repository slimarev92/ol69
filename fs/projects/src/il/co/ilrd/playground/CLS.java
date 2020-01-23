package il.co.ilrd.playground;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class CLS {
	public static void main(String[] args) {
		try(Socket httpSocket = new Socket("developer.twitter.com", 80);
			BufferedReader fileReader = new BufferedReader(new FileReader("/home/sasha/docs/request.txt"));
			OutputStreamWriter requestWriter = new OutputStreamWriter(httpSocket.getOutputStream());
			InputStreamReader responseReader = new InputStreamReader(httpSocket.getInputStream())) {
			StringBuilder builder = new StringBuilder(); 
		
			String currLine = null;
			
			while(null != (currLine = fileReader.readLine())) {
				builder.append(currLine + "\r\n");
			}
			
			builder.append("\r\n");
			
			System.out.println(builder.toString());
			
			System.out.println("--------------");
			
			requestWriter.write(builder.toString());
			requestWriter.flush();
			
			char[] response = new char[1024];
			
			responseReader.read(response);
			
			System.out.println(response);			
			
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
