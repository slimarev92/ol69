package il.co.ilrd.ws06;

import java.lang.reflect.Modifier;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class ObjectAnalyzer {
	public static void PrintAnalysis(String className) {
		Class<?> objClass = null;
		
		try {
			objClass = Class.forName(className);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		
		System.out.println(objClass.getSuperclass()); 
		System.out.println(Modifier.toString(objClass.getModifiers()));
		
		for(Class<?> curr : objClass.getInterfaces()) {
			System.out.println(curr);
		}
	}
	
	public static Method getMethodByName(Class<?> classObj, String methodName) {
				
		for(Method curr : classObj.getMethods()) {	
			if(curr.getName().equals(methodName)) {
				return curr;
			}
		}
		String className = classObj.getSimpleName();
		
		throw new IllegalArgumentException("Class " + className + 
				                           " has no method " + methodName);
	}
	
	public static Object createInsance(String className) {
		Object ret = null;
		
		try {
			ret = Class.forName(className).newInstance();
		} catch (Exception e) {
			e.printStackTrace();
		} 
		
		return ret;
	}
	
	public static Object invokeMethod(Object instance, String methodName, Object... args) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException{
		
		Method method = ObjectAnalyzer.getMethodByName(instance.getClass(),
				                                       methodName);
		
		return method.invoke(instance, args);
	}
	
	@SuppressWarnings("unused")
	public static void main(String[] args) {
		String className = "il.co.ilrd.complexnumber.ComplexNumber";
		String methodName = "toString";
		//Method method = ObjectAnalyzer.getMethodByName(Instance.class, methodName);

	}
}
