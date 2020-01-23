package il.co.ilrd.ws10;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

class DataObject {
	private String code;
	private int value;
	
	public DataObject(String code, int value) {
		this.code = code;
		this.value = value;
	}
	
	public String getCode() {
		return code;
	}
	
	public int getValue() {
		return value;
	}
	
	public void addToValue(int add) {
		value += add;
	}
}

public class Ex {
	public static void main(String[] args) {
		/* one */
		int[] arr = {5, 4, 3, 2, 1};
		
		System.out.println(Arrays.toString(arr));
		
		List<Integer> list = new ArrayList<>();
		
		for(int curr : arr) {
			list.add(curr);
		}
		
		Collections.sort(list);
		System.out.println(Arrays.toString(list.toArray()));
		
		/* two */
		Map<String, Integer> days = new TreeMap<>();
		
		days.put("Monday", 1);
		days.put("Tuesday", 2);
		days.put("Wednesday", 3);
		days.put("Thursday", 4);
		days.put("Friday", 5);
		days.put("Saturday", 6);
		days.put("Sunday", 7);
		
		for(Map.Entry<String, Integer> currEntry : days.entrySet()) {
			System.out.println(currEntry);
		}
		
		for(Integer currValue : days.values()) {
			System.out.println(currValue);
		}
		
		/* three */
		DataObject[] objs = {new DataObject("A", 1), 
				new DataObject("A", 1), 
				new DataObject("A", 1), 
				new DataObject("A", 1), 
				new DataObject("B", 1), 
				new DataObject("B", 1), 
				new DataObject("B", 1), 
				new DataObject("B", 1),
		};
		
		Map<String, DataObject> map = new HashMap<>();
		
		for(DataObject curr : objs) {
			DataObject currEntry = map.get(curr.getCode());
			
			if(null == currEntry) {
				map.put(curr.getCode(), new DataObject(curr.getCode(),
						                               curr.getValue()));
			} else {
				currEntry.addToValue(curr.getValue());
			}
		}

		for(Map.Entry<String, DataObject> curr : map.entrySet()) {
			System.out.println(curr.getKey() + " " + curr.getValue().getValue());
		}
	}
}
