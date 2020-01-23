package il.co.ilrd.factory;

import java.util.function.Function;

class DoesThings {
	private int number;
	
	public DoesThings(int number) {
		this.number = number;
	}
	
	public Integer getNumber() {
		return number;
	}
	
	public static DoesThings makeOne(Integer number) {
		return new DoesThings(number);
	}
	
	public DoesThings getCopy() {
		return new DoesThings(number);
	}
	
	public DoesThings makeOneSum(Integer add) {
		return new DoesThings(getNumber() + add);
	}
}

public class Tasks {

	private enum Options { LAMBDA, ANON, STATIC, INSTANCE_OBJECT, INSTANCE_TYPE};
	
	public static void main(String[] args) {
		Factory<Options, Integer, DoesThings> factory = new Factory<>();
		
		/* TASK 1 */
		
		factory.add(Options.LAMBDA, (num) -> new DoesThings(num));
		
		DoesThings person = factory.create(Options.LAMBDA, 23);
		
		System.out.println(person.getNumber());
		
		/* TASK 2 */
		
		factory.add(Options.ANON, new Function<Integer, DoesThings>() {

			@Override
			public DoesThings apply(Integer t) {
				return new DoesThings(t);
			}
		});
		
		person = factory.create(Options.ANON, 300);
		
		System.out.println(person.getNumber());
		
		/* TASK 3 */
		
		factory.add(Options.STATIC, DoesThings::makeOne);
		
		person = factory.create(Options.STATIC, 666);
		
		System.out.println(person.getNumber());
		
		/* TASK 4 */
		
		factory.add(Options.INSTANCE_OBJECT, person::makeOneSum);
		
		DoesThings person1 = factory.create(Options.INSTANCE_OBJECT, -666);
		
		System.out.println(person1.getNumber());
		
		/* TASK 5 */
		
		Factory<Options, DoesThings, DoesThings> factory2 = new Factory<>();
		
		factory2.add(Options.INSTANCE_TYPE, DoesThings::getCopy);
		
		DoesThings dt = new DoesThings(1000);
		
		System.out.println(factory2.create(Options.INSTANCE_TYPE, dt).getNumber());
	}
}
