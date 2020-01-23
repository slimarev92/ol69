package il.co.ilrd.playground;

import java.sql.Connection;
import java.sql.DriverManager;

class JDBCTest {

	private static final String url = "jdbc:mysql://localhost";

	private static final String user = "sasha";

	private static final String password = "uhf2xdja";

	public static void main(String args[]) {
		
		try {
			Connection con = DriverManager.getConnection(url, user, password);
			System.out.println("Success");

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

