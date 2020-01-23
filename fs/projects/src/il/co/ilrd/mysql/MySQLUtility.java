package il.co.ilrd.mysql;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;

public class MySQLUtility implements AutoCloseable {
	private final Connection mysqlConnection;
	private final Properties connectionProperties = new Properties();
	private final Statement mysqlStatement; 
	
	public MySQLUtility(String url, String user, String password) throws SQLException {
		connectionProperties.put("user", user);
		connectionProperties.put("password", password);
		
		mysqlConnection = DriverManager.getConnection("jdbc:mysql://" + url + "?useSSL=false", connectionProperties);
		mysqlStatement = mysqlConnection.createStatement();
	}
	
	public boolean createDB(String database) throws SQLException {
		return mysqlStatement.execute("create database " + database + ";");
	}
	
	public boolean useDB(String database) throws SQLException {
		return mysqlStatement.execute("use " + database + ";");
	}
	
	public boolean execute(String query) throws SQLException {
		return mysqlStatement.execute(query);
	}
	
	public int executeUpdate(String query) throws SQLException {
		return mysqlStatement.executeUpdate(query);
	}
	
	public ResultSet executeQuery(String query) throws SQLException {
		return mysqlStatement.executeQuery(query);
	}
	
	@Override
	public void close() throws SQLException {
		mysqlStatement.close();
		mysqlConnection.close();
	}
}	