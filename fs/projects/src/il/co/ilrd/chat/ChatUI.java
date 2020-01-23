package il.co.ilrd.chat;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import il.co.ilrd.concurrency.WaitableQueue;

public class ChatUI {
	private final JTextArea chatArea = new JTextArea("Welcome to LimChat\n");
	private final JFrame outerContainer = new JFrame("LimChat");
	private final JButton submitButton = new JButton("Send");
	private final JTextArea inputField = new JTextArea();
	private final JPanel buttomPart = new JPanel();
	private final JPanel topPart = new JPanel();
	private final JScrollPane buttomScrollPane;
	private final JPanel left = new JPanel();
	private final JScrollPane topScrollPane;
	private volatile boolean open = true;
	
	public ChatUI(WaitableQueue<String> outgoingMessageQueue, WaitableQueue<String> incomingMessageQueue) {
		outerContainer.setLayout(new BorderLayout());
		outerContainer.setResizable(false);
		outerContainer.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		
		outerContainer.addWindowListener(new WindowListener() {
			
			@Override
			public void windowOpened(WindowEvent e) {}
			
			@Override
			public void windowIconified(WindowEvent e) {}
			
			@Override
			public void windowDeiconified(WindowEvent e) {}
			
			@Override
			public void windowDeactivated(WindowEvent e) {}
			
			@Override
			public void windowClosing(WindowEvent e) {}
			
			@Override
			public void windowClosed(WindowEvent e) {
				open = false;
				outgoingMessageQueue.enqueue("");
			}
			
			@Override
			public void windowActivated(WindowEvent e) {}
		});
		
		outerContainer.setSize(800, 600);
				
		submitButton.setPreferredSize(new Dimension(100, 110));
		
		inputField.setFont(new Font("Free Sans", Font.PLAIN, 16));
		inputField.setLineWrap(true);
		inputField.addKeyListener(new KeyListener() {
			
			@Override
			public void keyTyped(KeyEvent e) {}
			
			@Override
			public void keyReleased(KeyEvent e) {
				if(e.getKeyCode() == 10) {
					inputField.setCaretColor(Color.BLACK);
					inputField.setText("");
				}
			}
			
			@Override
			public void keyPressed(KeyEvent e) {
				if(e.getKeyCode() == 10) {
					String currInput = inputField.getText();
					inputField.setText("");
					
					inputField.setCaretColor(Color.WHITE);					
					
					outgoingMessageQueue.enqueue(currInput);
				}		
			}
		});
		
		topPart.setPreferredSize(new Dimension(800, 450));
		topPart.setLayout(new BorderLayout());
		
		chatArea.setMargin(new Insets(10, 10, 10, 10));
		chatArea.setEditable(false);
		chatArea.setFont(new Font("Free Sans", Font.PLAIN, 16));
		chatArea.setLineWrap(true);
			
		topScrollPane = new JScrollPane(chatArea);
		
		topPart.add(topScrollPane, BorderLayout.CENTER);
		
		buttomScrollPane = new JScrollPane(inputField);
		
		left.setLayout(new BorderLayout());
		left.setPreferredSize(new Dimension(680, 110));
		left.add(buttomScrollPane, BorderLayout.CENTER);
		
		buttomPart.setPreferredSize(new Dimension(800, 110));
		buttomPart.setLayout(new FlowLayout());
		buttomPart.add(left);
		buttomPart.add(submitButton);
		
		outerContainer.getContentPane().add(buttomPart, BorderLayout.WEST);
		outerContainer.getContentPane().add(topPart, BorderLayout.NORTH);
	
		submitButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if(e.getActionCommand().equals("Send")) {
					String currInput = inputField.getText();
					outgoingMessageQueue.enqueue(currInput);
					inputField.setText("");
				}
			}
		});
	
		outerContainer.setVisible(true);
	}
	
	public boolean isOpen() {
		return open;
	}
	
	public void displayMessage(String message) {
		chatArea.append(message + "\n");
	}
}