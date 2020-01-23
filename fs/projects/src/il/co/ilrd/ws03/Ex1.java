package il.co.ilrd.ws03;

class Human {
	
	private String name;
	
	static {
		System.out.println("Human - static init block");
	}
	
	{
		System.out.println("Human - first init block");
	}
	
	public String getName() {
		return name;
	}
	
	public Human(String name) {
		this.name = "human " + name;
		System.out.println("Human - constructor");
	}
	
	{
		System.out.println("Human - second init block");
	}
}

class Commoner extends Human {
	
	static {
		System.out.println("Commoner - static init block");
	}
	
	{
		System.out.println("Commoner - first init block");
	}
	
	public Commoner(String name) {
		super("commoner " + name);
		System.out.println("Commoner - constructor");
	}
	
	{
		System.out.println("Commoner - second init block");
	}
}

class King extends Human {
	
	static {
		System.out.println("King - static init block");
	}
	
	{
		System.out.println("King - first init block");
	}
	
	public King(String name) {
		super("king " + name);
		System.out.println("Commoner - constructor");
	}
	
	{
		System.out.println("King - second init block");
	}
}

public class Ex1 {

	public static void main(String[] args) {
		
		Human h = new Commoner("Sasha");
		
		System.out.println("------------------");
		
		King k = new King("Rex");
		
		System.out.println("------------------");
		
		System.out.println(h.getName());
		
		System.out.println("------------------");
		
		System.out.println(k.getName());
	}
}
