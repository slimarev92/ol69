package il.co.ilrd.vendingmachine;

public class Product {
	private final String name;
	private double price;
	
	public Product(String name, double price) {
		this.name = name;
		this.price = price;
	}
	
	public String getName() {
		return name;
	}
	
	public double getPrice() {
		return price;
	}
	
	public void setPrice(double price) {
		this.price = price;
	}
	
	@Override
	public String toString() {
		return getName() + ", priced at " + getPrice();
	}
	
	@Override
	public boolean equals(Object obj) {
		
		if(!(obj instanceof Product)) {
			return false;
		}
	
		return name.equals(((Product)obj).name);
	}
}