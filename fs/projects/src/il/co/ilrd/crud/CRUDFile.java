package il.co.ilrd.crud;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class CRUDFile implements CRUD<Integer, String> {
	private final BufferedWriter writer; 
	
	public CRUDFile(String dest) throws IOException {
		File destFile = new File(dest);
		
		if(!destFile.isFile()) { throw new IllegalArgumentException(); }
		
		destFile.createNewFile();
		
		writer = new BufferedWriter(new FileWriter(destFile, true));
	}
	
	@Override
	public void close() throws Exception {
		writer.close();
	}

	@Override
	public Integer create(String entry) {
		try {
			System.out.println("writing |" + entry + "|");
			writer.append(entry);
			writer.newLine();
			writer.flush();
		} catch (IOException e) {
			return null;
		}
		
		return 1;
	}

	@Override
	public String read(Integer key) {
		throw new UnsupportedOperationException();
	}

	@Override
	public void update(Integer key, String data) {
		throw new UnsupportedOperationException();
		
	}

	@Override
	public void delete(Integer key) {
		throw new UnsupportedOperationException();		
	}
	
}
