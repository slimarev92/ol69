package il.co.ilrd.ws14;

import java.awt.datatransfer.StringSelection;

public class CalcProtocol {
	private State currState = State.WAITING_NUM;
	private int[] operands = new int[2];
	private int index = 0;
	
	public String proccessInput(String input) {
		
		input = input.replace("\n", "");
		System.out.println("input is " + input);
		
		if(currState == State.WAITING_NUM) {
			try {
				operands[index] = Integer.parseInt(input);
				
				if(0 == index) {
					currState = State.WAITING_OPERATOR;
					index = 1;
					
					return "WAITING FOR OPERATOR";
				} else {
					index = 0;
					return "THE RESULT IS: " + (operands[0] + operands[1]) + ", " + "WAITING FOR OPERAND";
				}	
			} catch(NumberFormatException e) {
				return "ERROR: INVALID OPERAND";
			}
		} else {
			if(!input.equals("+")) {
				return "ERROR: INVALID OPERATOR";
			} else {
				currState = State.WAITING_NUM;
				return "WAITING FOR OPERAND";
			}
		}
	}
	
	private enum State { WAITING_NUM, WAITING_OPERATOR };
}
