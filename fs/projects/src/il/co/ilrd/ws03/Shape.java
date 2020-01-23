package il.co.ilrd.ws03;

public abstract class Shape {
	private String color;
	private boolean filled;
	
	public Shape() {
		color = "green";
		filled = true;
	}
	
	public Shape(String color, boolean filled) {
		this.color = color;
		this.filled = filled;
	}
	
	public String getColor() {
		return color;
	}
	
	public void setColor(String updatedColor) {
		color = updatedColor;
	}
	
	public boolean isFilled() {
		return filled;
	}
	
	public void setFilled(boolean updatedValue) {
		filled = updatedValue;
	}
	
	public abstract double getArea();
	public abstract double getPerimeter();

	@Override
	public String toString() {
		String retVal =  "A " + getColor() + " colored shape that is ";
		String addition = filled ? "filled" : "not filled";
		
		return retVal + addition;
	}
}
