package il.co.ilrd.logmonitor;

import java.io.IOException;
import java.net.ProtocolException;

public class LogClient {
	public static void main(String[] args) {
		try {
			LogMonitor monitor = new LogMonitor("/var/log/syslog");
			ClientLogProtocol protocol = new ClientLogProtocol("localhost", 7777);
			
			monitor.registerExceptionHandler(event -> event.printStackTrace());
			
			monitor.registerEventHandler(event -> {
				try {
					protocol.sendMessage(event.getBytes());
				} catch (ProtocolException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			});
			
			monitor.start();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
