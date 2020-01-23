package il.co.ilrd.complexnumber;

import java.math.BigDecimal;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class ComplexNumber implements Comparable<ComplexNumber> {
    private double real;
    private double imaginary;
    public static final int HASH_RNG = 10000;
	private static final String SINGLE_NUMBER = "[0-9]+.[0-9]+";
	private static final String PLUS_MINUS = "[+-]?";
	private static final double ALMOST_ZERO = 0.001;

    public ComplexNumber() {
        this(0.0, 0.0);
    }

    public ComplexNumber(double real, double imaginary) {
        this.real = real;
        this.imaginary = imaginary;
    }

    public double getReal() {
        return real;
    }

    public double getImaginary() {
        return imaginary;
    }

    public void setReal(double real) {
        this.real = real;
    }

    public void setImaginary(double imaginary) {
        this.imaginary = imaginary;
    }

    public void setValue(double real, double imaginary) {
        this.real = real;
        this.imaginary = imaginary;
    }

    public ComplexNumber add(ComplexNumber other) {
        setReal(real + other.real);
        setImaginary(imaginary + other.imaginary);
        
        return this;
    }

    public ComplexNumber subtract(ComplexNumber other) {
    	  setReal(real - other.real);
    	  setImaginary(imaginary - other.imaginary);
    	  
    	  return this;
    }

    public ComplexNumber multiply(ComplexNumber other) {
        double a = this.real;
        double b = this.imaginary;
        double c = other.real;
        double d = other.imaginary;

        setReal(a * c - b * d);
        setImaginary(a * d + b * c);
        
        return this;
    }

    public ComplexNumber divide(ComplexNumber other) {
        double a = this.real;
        double b = this.imaginary;
        double c = other.real;
        double d = other.imaginary;

        setReal((a * c + b * d) / (Math.pow(c, 2) + Math.pow(d, 2)));
        setImaginary((b * c - a * d) / ((Math.pow(c, 2) + Math.pow(d, 2))));
        
        return this;
    }

    @Override
    public String toString() {
        String sign = null;
        BigDecimal bdReal = new BigDecimal(real);
        BigDecimal bdImag = new BigDecimal(imaginary);
        
        bdReal = bdReal.setScale(3, BigDecimal.ROUND_HALF_DOWN);
        bdImag = bdImag.setScale(3, BigDecimal.ROUND_HALF_DOWN);
        
        if(real == 0 && imaginary == 0) {
            return "";
        } else if(real == 0) {
            return bdImag.toString() + "i";
        } else if(imaginary == 0) {
            return bdReal.toString();
        } else {
            sign = 0 > imaginary ? " - " : " + ";

            return bdReal.toString() + sign + bdImag.abs().toString() + "i";
        }
    }

    @Override
    public boolean equals(Object number) {
    	
    	ComplexNumber compNum = null;
    	
    	if(!(number instanceof ComplexNumber)) {
    		return false;
    	}
    	
        compNum = (ComplexNumber) number;
   
        return ALMOST_ZERO >= Math.abs(this.real - compNum.real) &&
        	   ALMOST_ZERO >= Math.abs(this.imaginary - compNum.imaginary);
    }

    @Override
    public int hashCode() {
        return ((int)Math.round(real) * (int)Math.round(imaginary)) % HASH_RNG;
    }
     
    @Override
    public int compareTo(ComplexNumber num) {
        return (int)((0.5 * (real - num.real)) +
        		     (0.5 * (imaginary - num.imaginary)));
    }
    
    public static ComplexNumber parseString(String str) {
    	String strNoWs = str.replaceAll("\\s*", "");
    	
    	Pattern fullPattern = Pattern.compile(PLUS_MINUS + SINGLE_NUMBER + 
    			                            PLUS_MINUS + SINGLE_NUMBER + "i");
    	Pattern singleNumPat = Pattern.compile(PLUS_MINUS + SINGLE_NUMBER);
    	Matcher matchFull = fullPattern.matcher(strNoWs);
    	Matcher matchSingle = singleNumPat.matcher(strNoWs);
    	ComplexNumber ret = new ComplexNumber();
    	
    	if(!matchFull.matches()) {
    		return null;
    	}
    	
    	matchSingle.find();
    	ret.setReal(Double.parseDouble(matchSingle.group()));
    	
     	matchSingle.find();
    	ret.setImaginary(Double.parseDouble(matchSingle.group()));
    	
		return ret;
	}
}