package il.co.ilrd.ws05;

import java.util.function.Predicate;
import java.util.function.Consumer;

class Dog {
	String name;
	String coatDescrption;
	int age;
	
	public Dog(String name, String coatDescrption, int age) {
		this.name = name;
		this.age = age;
		this.coatDescrption = coatDescrption;
	}
	
	public static void actionOnMatching(Dog[] dogs,
			                     Predicate<Dog> tester,
			                     Consumer<Dog> action) {
		for(Dog dog : dogs) {
			if(true == tester.test(dog)) {
				action.accept(dog);
			}
		}
	}
	
	@Override
	public String toString() {
	
		return "A dog named " + name + ", age " + age;
	}
}

public class LambdaExp {
	
	public static void main(String[] args) {
		Dog[] dogsIKnow = { new Dog("Olive", "Black", 1), 
		           new Dog("Becky", "Orange", 10),
		           new Dog("Georgia", "Black and white", 11),
		           new Dog("Nehama", "Brownish", 5),
		           new Dog("Billie", "Light brown", 12),
				   new Dog("Dvora", "Brown and white", 6),
				   new Dog("Choopi", "Orange", 14),
				   new Dog("Beija", "Beige", 8) 
		};
		
		Dog.actionOnMatching(dogsIKnow, 
				             (Dog dog) -> dog.age > 5 && dog.age < 12,
				             (Dog dog) -> System.out.println(dog));
	}
}
