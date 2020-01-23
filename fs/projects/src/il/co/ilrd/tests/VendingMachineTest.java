package il.co.ilrd.tests;

import il.co.ilrd.vendingmachine.VendingMachine;
import il.co.ilrd.vendingmachine.Product;

public class VendingMachineTest {
	public static void main(String[] args) {
		Product cola = new Product("Coca Cola", 3);
		@SuppressWarnings("unused")
		Product superDrink = new Product("Super drink", 1);
		VendingMachine machine = new VendingMachine();
		machine.addProduct(cola);
		
		System.out.println("Test 1: -----------------");
		
		machine.chooseProduct("Red wine"); //please start machine
		machine.start(); //started machine
		machine.insertMoney(1); //inserted 1
		machine.chooseProduct(cola.getName()); //please insert more money
		System.out.println("Current balance: $" + machine.getBalance()); //0
		machine.chooseProduct("Red wine"); // choose an existing product
		machine.insertMoney(3);
		machine.chooseProduct("Coca Cola"); // sold
		
		System.out.println("\nTest 2: -----------------");
		
		machine.chooseProduct(cola.getName()); //insert money
		machine.insertMoney(100); //inserted 
		machine.chooseProduct(cola.getName()); //sold
		
		System.out.println("\nTest 3: ------------------");
		
		machine.start(); //already started
		machine.stop(); //stopped
		machine.addProduct(new Product("Fuze Tea", 5)); // added product
		machine.insertMoney(100000); //please start machine
		machine.start(); //started machine
		machine.insertMoney(1000); // inserted 1000
		machine.chooseProduct("Fuze Tea"); //sold 
		machine.stop(); //stopped machine 
		
		System.out.println("\nTest 4: -------------------");
		
		machine.start(); //started machine
		machine.insertMoney(100); //inserted money
		
		try {
			System.out.println("Going to sleep");
			Thread.sleep(4300); 
		}
		catch(InterruptedException e) {
			//do nothing?
		}
		
		System.out.println("Woke up, trying to choose product");
		
		machine.chooseProduct("Coca Cola"); // timed out
		machine.insertMoney(100); //added money
		machine.chooseProduct("Coca Cola"); //sold
		machine.stop(); //stopped
		
		System.out.println("\nTest 5: -------------------");
		
		machine.start(); //started machine
		machine.insertMoney(100); //inserted money
		
		try {
			System.out.println("Going to sleep");
			Thread.sleep(100); // Ran out of time, try again  
		}
		catch(InterruptedException e) {
			//do nothing?
		}
		
		System.out.println("Woke up, trying to choose product");
		machine.insertMoney(100); //inserted money
		
		machine.chooseProduct("Coca Cola"); // sold
		machine.chooseProduct("Coca Cola"); // insert money
		machine.stop();
		machine.stop();
	}
}
