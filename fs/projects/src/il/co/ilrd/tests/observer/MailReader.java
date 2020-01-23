package il.co.ilrd.tests.observer;

import il.co.ilrd.observer.Callback;

public class MailReader {
	private String name;
	private Callback<MailItem> mailCallback;
	private int receivedItems;
	
	public MailReader(String name) {
		this.name = name;
		mailCallback = new Callback<MailItem>(this::readMail, this::postOfficeClosed);
	}
	
	public void readMail(MailItem message) {
		++receivedItems;
		System.out.println(name + " reading mail: " + message.getMessage());
	}
	
	public void registerAtPostOffice(PostOffice po) {
		po.addCostumer(this);
	}
	
	public void postOfficeClosed() {
		System.out.println("the post office closed, no more mail for " + name + ":(");
	}
	
	public void leavePostOffice() {
		mailCallback.unsubscribe();
	}
	
	public Callback<MailItem> getCallback() {
		return mailCallback;
	}
	
	public int getReceivedItems() {
		return receivedItems;
	}
}
