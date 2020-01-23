package il.co.ilrd.vendingmachine;

import java.util.List;
import java.util.ArrayList;

public class VendingMachine {
	private volatile int balance;
	private volatile VendingMachineState state;
	private List<Product> products;
	private Monitor monitor;
	private Thread metronome;
	private int remainingTime;

	{
		products = new ArrayList<Product>();
		monitor = new MonitorImpl();
		balance = 0;
		state = VendingMachineState.INIT;
		metronome = null;
		remainingTime = 0;
	}
	
	public VendingMachine(Product... products) {	
		for(Product curr : products) {
			this.products.add(curr);
		}		
	}
	
	public int getBalance() {
		return balance;
	}

	private Monitor getMonitor() {
		return monitor;
	}
	
	private Product getProduct(String productName) {
		for (Product curr : getProducts()) {
			if (curr.getName().equals(productName)) {
				return curr;
			}
		}

		return null;
	}
	
	private int getRemainingTime() {
		return remainingTime;
	}

	private void setBalance(int balance) {
		this.balance = balance;
	}

	private synchronized void setState(VendingMachineState state) {
		this.state = state;
	}
	
	private void changeRemainingTime(int delta) {
		remainingTime += delta;
	}
	
	private void setRemainingTime(int remainingTime) {
		this.remainingTime = remainingTime;
	}
	
	public List<Product> getProducts() {
		return products;
	}
	
	public void addProduct(Product product) {
		if(VendingMachineState.INIT != state) {
			getMonitor().print(("Please stop machine"));
			
			return;
		}
		
		products.add(product);
		monitor.print("Added product: " + product.getName());
	}
	
	public void addProducts(Product... products) {
		for(Product curr : products) {
			this.products.add(curr);
			monitor.print("Added product: " + curr.getName());
		}
	}
	
	private void addToBalance(int amount) {
		balance += amount;
	}

	private void stopMetronome() {
		if(null != metronome) {
			metronome.interrupt();			
		}
	}
	
	private void startMetronome() {
		metronome = new Thread(new Metronome(this));
		metronome.start();
	}
	
	public void insertMoney(int amount) {
		state.insertMoney(this, amount);
	}

	public void chooseProduct(String productName) {
		state.choose(this, productName);
	}

	private void checkTimeout() {
		state.checkTimeout(this);
	}
	
	public void start() {
		state.start(this);
	}

	public void stop() {
		state.stop(this);
	}

	private enum VendingMachineState {
		WAIT_MONEY (0) {
			@Override
			public void insertMoney(VendingMachine machine, int amount) {
				machine.addToBalance(amount);
				machine.setState(VendingMachineState.WAIT_CHOICE);
				machine.getMonitor().print("Inserted $" + amount);
				machine.setRemainingTime(WAIT_CHOICE.getTimeout());
			}

			@Override
			public void choose(VendingMachine machine, String productName) {
				machine.getMonitor().print("Please insert money");
			}

			@Override
			public void start(VendingMachine machine) {
				machine.getMonitor().print("Machine already on");
			}
		},
		WAIT_CHOICE (3) {
			@Override
			public void insertMoney(VendingMachine machine, int amount) {
				machine.addToBalance(amount);
				machine.getMonitor().print("Inserted $" + amount);
				
				machine.setRemainingTime(getTimeout());
			}

			@Override
			public void choose(VendingMachine machine, String productName) {
				Product product = machine.getProduct(productName);
				
				if (null == product) {
					machine.getMonitor().print("Choose an existing product!");
				} else if (machine.getBalance() < product.getPrice()) {
					machine.setBalance(0);
					machine.getMonitor().print("Please insert $" + 
					                 product.getPrice() + ". Returning money.");
				} else {
					machine.setBalance(0);
					machine.getMonitor().print("Sold: " + product.getName());
					machine.setState(VendingMachineState.WAIT_MONEY);
				}
			}

			@Override
			public void start(VendingMachine machine) {
				machine.getMonitor().print("Vending machine already on");
			}
			
			@Override
			public void checkTimeout(VendingMachine machine) {
				machine.changeRemainingTime(-1);
				
				if(0 == machine.getRemainingTime()) {
					machine.setBalance(0);
					machine.setState(WAIT_MONEY);
					machine.getMonitor().print("Out of time, refunding");
				} 
			}
		},
		INIT (0) {
			@Override
			public void insertMoney(VendingMachine machine, int amount) {
				machine.getMonitor().print("Please start machine");
			}

			@Override
			public void choose(VendingMachine machine, String productName) {
				machine.getMonitor().print("Please start machine");
			}

			@Override
			public void start(VendingMachine machine) {
				machine.setState(WAIT_MONEY);
				machine.getMonitor().print("Starting machine");
				machine.startMetronome();
			}
		};

		private final int timeout;
		
		private VendingMachineState(int timeout) {
			this.timeout = timeout;
		}
		
		protected int getTimeout() {
			return timeout;
		}
		
 		public abstract void insertMoney(VendingMachine machine, int amount);
		public abstract void choose(VendingMachine machine, String productName);
		public abstract void start(VendingMachine machine);
		
		public void checkTimeout(VendingMachine machine) {}
		
		public void stop(VendingMachine machine) {
			machine.setBalance(0);
			machine.setState(VendingMachineState.INIT);
			machine.getMonitor().print("Stopping machine");
			machine.stopMetronome();
		}
	}
	
	private static class Metronome implements Runnable {
		private VendingMachine machine;
		
		public Metronome(VendingMachine machine) {
			this.machine = machine;
		}
		
		@Override
		public void run() { 
			while(true) {
				if(Thread.interrupted()) {
					return;
				}
				
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					return;
				}
				
				machine.checkTimeout();
			}
		}
	}
	
	private class MonitorImpl implements Monitor {
		@Override
		public void print(String message) {
			System.out.println(message);
		}
	}
}