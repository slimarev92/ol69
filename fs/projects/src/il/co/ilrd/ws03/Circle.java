package il.co.ilrd.ws03;

import static java.lang.Math.PI;

/** 
 * 
 * @author Sasha Limarev, slimarev@outlook.com
 *
 */

public class Circle extends Shape {
	private double radius;
	
	public Circle() {
		super();
		
		radius = 1.0;
	}
	
	public Circle(double radius) {
		super();
		
		this.radius = radius;
	}
	
	public Circle(double radius, String color, boolean filled) {
		super(color, filled);
		
		this.radius = radius;
	}
	
	public double getRadius() {
		return radius;
	}
	
	public void setRadius(double updatedRadius) {
		radius = updatedRadius;
	}
	
	@Override
	public double getArea() {
		return PI * radius * radius;
	}
	
	@Override
	public double getPerimeter() {
		return PI * 2 * radius;
	}
	
	@Override
	public String toString() {
		StringBuffer buff = new StringBuffer("A Circle with radius=" );
		
		buff.append(getArea());
		buff.append(", which is subclass of: ");
		buff.append(super.toString());
		
		return buff.toString();
	}
	
	public static void main(String[] args) {
		Circle c = new Circle();
		
		System.out.println(c.toString());
	}
}
