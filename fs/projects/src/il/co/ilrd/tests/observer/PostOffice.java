package il.co.ilrd.tests.observer;

import il.co.ilrd.observer.*;

public class PostOffice {
	Dispatcher<MailItem> dispatcher = new Dispatcher<MailItem>();
	
	public void addCostumer(MailReader customer) {
		dispatcher.subscribe(customer.getCallback());
	}
	
	public void addCostumers(MailReader... customers) {
		for(MailReader curr : customers) {
			dispatcher.subscribe(curr.getCallback());
		}
	}
	
	public void sendMail(MailItem message) {
		dispatcher.notify(message);
	}
	
	public void goBankrupt() {
		dispatcher.stop();
	}	
	
}
