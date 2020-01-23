package il.co.ilrd.crud;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import il.co.ilrd.logmonitor.LogMonitor;
import il.co.ilrd.mysql.MySQLUtility;

public class MysqlLogger implements CRUD<Long, String> {
	private final MySQLUtility mysqlUtil;
	private long idCounter = 0;

	public MysqlLogger(String databaseURL, String user, String password) throws SQLException {	
		mysqlUtil = new MySQLUtility(databaseURL, user, password);
					
		mysqlUtil.execute("create table if not exists Log (id int primary key not null, event varchar(255));");
		
		try (ResultSet numOfRecords = mysqlUtil.executeQuery("select max(id) from Log");) {
			if (false != numOfRecords.next()) {
				idCounter = numOfRecords.getLong(1) + 1;
			}
		}
	}
	
	@Override
	public void close() throws Exception {
		mysqlUtil.close();
	}

	@Override
	public Long create(String entry) {
		long eventID = idCounter++;
		
		try {		
			mysqlUtil.execute("insert into Log values (" + eventID + ", '" + addQuoteEscape(entry) + "');");
		} catch (SQLException e) {
			e.printStackTrace();
			return (long)-1;
		}
		
		return eventID;
	}

	@Override
	public String read(Long key) {
		String logLine = null;
		
		try {
			try(ResultSet res = mysqlUtil.executeQuery("select event from Log where id=" + key + ";")) {
				res.next();
				logLine = res.getString("event");
			}
		} catch (SQLException e) { 
			e.printStackTrace();
		} 
		
		return logLine;
	}

	@Override
	public void update(Long key, String newEntry) {		
		try {
			mysqlUtil.execute("update Log set event='" + addQuoteEscape(newEntry) + "' where id= " + key +";");
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void delete(Long key) {
		try {
			mysqlUtil.execute("delete from Log where id=" + key + ";");
		} catch (SQLException e) {
			e.printStackTrace();
		}	
	}
	
	private String addQuoteEscape(String entry) {
		return entry.replaceAll("'", "''");
	}
	
	public static void main(String[] args) {
		try {
			LogMonitor monitor = new LogMonitor("/var/log/syslog");
			MysqlLogger test = new MysqlLogger("localhost:3306/logging", "sasha", "uhf2xdja");
			
			System.out.println("Connecting to database...");
			System.out.println("Starting log monitor...");
			
			monitor.registerEventHandler(event-> System.out.println("Created entry with id " + test.create(event)));
			
			try {
				monitor.start();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
			
			test.update((long)10, "this entry was hacked by S4SH4"); 
			
			System.out.println(test.read((long)10)); /* should print the modified entry */
		
			test.delete((long)10);
			
			System.out.println(test.read((long)10)); /* should print null and throw an exception */
		
			try {
				Thread.sleep(Long.MAX_VALUE);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
			try {
				test.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
			
		} catch (SQLException e) {
			e.printStackTrace();
		}
			
		
	}
}
