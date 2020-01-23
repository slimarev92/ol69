package il.co.ilrd.ws05;

public class Outer {

	public void stuff() {
		@SuppressWarnings("unused")
		int a = 40;
		
		class Local {
			int a = 20;
			
			public  void stufff() {
				System.out.println(a);
			}
		};
		
		Local l = new Local();
		
		l.stufff();
		
	}
	
	public static void main(String[] args) {
		Outer o = new Outer();
		o.stuff();
	}
	
}
