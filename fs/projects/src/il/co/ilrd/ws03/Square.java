package il.co.ilrd.ws03;

import java.lang.StringBuffer;

public class Square extends Rectangle {
	
	public Square() {
		super();
	}
	
	public Square(double side) {
		super(side, side);
	}
	
	public Square(double side, String color, boolean filled) {
		super(side, side, color, filled);
	}
	
	public double getSide() {
		return getWidth();
	}
	
	public void setSide(double side) {
		setWidth(side);
		setLength(side);
	}
	
	@Override
	public void setWidth(double width) {
		setSide(width);
	}
	
	@Override
	public void setLength(double length) {
		setSide(length);
	}
	@Override
	public String toString() {
	
		StringBuffer buff = new StringBuffer("A square, "
				                             + "which is a subclass of: ");
		buff.append(super.toString());
		
		return buff.toString();
	}
	
	public static void main(String[] args) {
		Square sq = new Square(11.23);
		
		System.out.println(sq.toString());
	}

}
