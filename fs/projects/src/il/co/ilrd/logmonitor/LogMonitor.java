package il.co.ilrd.logmonitor;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.LinkedList;
import java.util.List;
import java.util.function.Consumer;

public class LogMonitor {
    private final List<Consumer<Exception>> exceptionHandlers = new LinkedList<>();
    private final List<Consumer<String>> eventHandlers = new LinkedList<>();
    private volatile boolean wasStopped = false;
    private final MonitorThread monitorThread;
    private boolean wasStarted = false;
    private WatchService service;
    private final String source;

    public LogMonitor(String source) {
    	if(!new File(source).isFile()) { throw new IllegalArgumentException(); }
    	
    	this.source = source;
    	monitorThread = new MonitorThread();
    }

    public void start() throws IOException {
    	checkLegalStateBeforeStart();
    	
    	service = FileSystems.getDefault().newWatchService();
    	Path sourcePath = Paths.get(source).getParent();
    	sourcePath.register(service, StandardWatchEventKinds.ENTRY_MODIFY);
    	
    	monitorThread.start();
    	wasStarted = true;
    }
    
    public void registerEventHandler(Consumer<String> onEvent) {
    	checkLegalStateBeforeStart();
    	if(null == onEvent) { throw new IllegalArgumentException(); }
    	
        eventHandlers.add(onEvent);
    }
    
    public void registerExceptionHandler(Consumer<Exception> onException) {
    	checkLegalStateBeforeStart();
    	if(null == onException) { throw new IllegalArgumentException(); }
    	
    	exceptionHandlers.add(onException);
    }

    public void stop() throws IOException, InterruptedException {
    	if(!wasStarted || wasStopped) { throw new IllegalStateException(); }
    	
    	wasStopped = true;
    	monitorThread.interrupt();
    	monitorThread.join();
    	service.close();
    }    
    
    private void checkLegalStateBeforeStart() {
    	if(wasStarted || wasStopped) { throw new IllegalStateException(); }
    }
    
    private class MonitorThread extends Thread {
    	private final Path fileName = Paths.get(source).getFileName();
    	private String currLine = null;
        private int readOffset = 0;
    	
        @Override
        public void run() {
            while(!wasStopped) {
            	try {
					WatchKey key = service.take();
					
					for(WatchEvent<?> event : key.pollEvents()) {
						if(event.context().equals(fileName)) {
							handleEvent();
							break;
						}
					}
					
					key.reset();
				} catch(InterruptedException e) {
					break;
				} catch(Exception e) {
					exceptionHandlers.forEach(handler -> handler.accept(e));
				}
            }
        }
        
        private void handleEvent() throws IOException {
        	try(BufferedReader reader = new BufferedReader(new FileReader(source))) {
				reader.skip(readOffset);
						
				while(null != (currLine = reader.readLine())) {
					eventHandlers.forEach(handler -> handler.accept(currLine));
					readOffset += currLine.length() + 1;
				}
			}
        }
    }    
}