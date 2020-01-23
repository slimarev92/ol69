package il.co.ilrd.ws03;

import java.lang.StringBuffer;

public class Rectangle extends Shape {
	private double width;
	private double length;
	
	public Rectangle() {
		super();
		
		width = 1.0;
		length = 1.0;
	}
	
	public Rectangle(double width, double length) {
		super();
		
		this.width = width;
		this.length = length;
	}
	
	public Rectangle(double width,
			         double length,
			         String color,
			         boolean filled) {
		super(color, filled);
		
		this.width = width;
		this.length = length;
	}
	
	public double getWidth() {
		return width;
	}

	public double getLength() {
		return length;
	}
	
	public void setWidth(double updatedWidth) {
		width = updatedWidth;
	}
	
	public void setLength(double updatedLength) {
		length = updatedLength;
	}
	
	@Override
	public double getPerimeter() {
		return (width * 2) + (length * 2);
	}
	
	@Override
	public double getArea() {
		return width * length;
	}
	
	@Override
	public String toString() {
		
		StringBuffer buff = new StringBuffer("A rectangle with width=");
		buff.append(getWidth() + " and length=" + getLength());
		buff.append(", which is a subclass of: " + super.toString());
		
		return buff.toString();
	}
	
	public static void main(String[] args) {
		Rectangle rect = new Rectangle(20.0, 30.0);
		
		System.out.println(rect.toString());
	}
}