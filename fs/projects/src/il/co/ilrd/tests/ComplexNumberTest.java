package il.co.ilrd.tests;

import org.junit.Test;
import org.junit.Assert;

import il.co.ilrd.complexnumber.ComplexNumber;

public class ComplexNumberTest {
	private ComplexNumber num = null;
	private ComplexNumber secondNum = null;
	private ComplexNumber expected = null;

	@Test
	public void TestAdd1() {
		num = new ComplexNumber(1.0, 1.0);
		secondNum = new ComplexNumber(3.0, 5.0);
		expected = new ComplexNumber(4.0, 6.0);

		num.add(secondNum);

		Assert.assertEquals(expected, num);
		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestAdd2() {
		num = new ComplexNumber(1.23, 5.0);
		secondNum = new ComplexNumber(-1.13, 2.0);
		expected = new ComplexNumber(0.1, 7.0);

		num.add(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestAdd3() {
		num = new ComplexNumber(0, 0);
		secondNum = new ComplexNumber(-1.13, 2.0);
		expected = new ComplexNumber(-1.13, 2.0);

		num.add(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestSub1() {
		num = new ComplexNumber(1.0, 1.0);
		secondNum = new ComplexNumber(3.0, 5.0);
		expected = new ComplexNumber(-2.0, -4.0);

		num.subtract(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestSub2() {
		num = new ComplexNumber(1.23, 5.0);
		secondNum = new ComplexNumber(-1.13, 2.0);
		expected = new ComplexNumber(2.36, 3.0);

		num.subtract(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestSub3() {
		num = new ComplexNumber(0, 0);
		secondNum = new ComplexNumber(-1.13, 2.0);
		expected = new ComplexNumber(1.13, -2.0);

		num.subtract(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestDiv1() {
		num = new ComplexNumber(1.0, 1.0);
		secondNum = new ComplexNumber(3.0, 5.0);
		expected = new ComplexNumber(0.235294, -0.0588235);

		num.divide(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestDiv2() {
		num = new ComplexNumber(1.23, 5.0);
		secondNum = new ComplexNumber(2.13, 3.0);
		expected = new ComplexNumber(1.30162, 0.514150);

		num.divide(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestMul1() {
		num = new ComplexNumber(1.1, 1.1);
		secondNum = new ComplexNumber(3.0, 5.0);
		expected = new ComplexNumber(-2.199, 8.8);

		num.multiply(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestMul2() {
		num = new ComplexNumber(1.23, 5.0);
		secondNum = new ComplexNumber(-1.13, 2.0);
		expected = new ComplexNumber(-11.3899, -3.19);

		num.multiply(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestMult3() {
		num = new ComplexNumber(0, 0);
		secondNum = new ComplexNumber(-1.13, 2.0);
		expected = new ComplexNumber(0, 0);

		num.multiply(secondNum);

		Assert.assertEquals(expected, num);
	}

	@Test
	public void TestStr1() {
		num = new ComplexNumber(1.15, 0.23);
		String expected = "1.150 + 0.230i";

		Assert.assertEquals(expected, num.toString());
	}

	@Test
	public void TestStr2() {
		num = new ComplexNumber(11.15, 0.23);
		String expected = "11.150 + 0.230i";

		Assert.assertEquals(expected, num.toString());
	}

	@Test
	public void TestStr3() {
		num = new ComplexNumber(0, 0.23);
		String expected = "0.230i";

		Assert.assertEquals(expected, num.toString());
	}

	@Test
	public void TestStr4() {
		num = new ComplexNumber(11.15, 0);
		String expected = "11.150";

		Assert.assertEquals(expected, num.toString());
	}
}