package il.co.ilrd.exam;

import java.util.List;
import java.util.ArrayList;

class Graph {
	private List<GraphNode> nodes = new ArrayList<GraphNode>();

	public void insert(int ID, char type, int... conenctions) {
		nodes.add(new GraphNode(ID, type, conenctions));
	}

	public void updateConnections() {
		for (GraphNode node : nodes) {
			for (int connection : node.connections) {
				getByID(connection).addConnection(node.getID());
			}
		}
	}

	public void insert(int ID, char type) {
		nodes.add(new GraphNode(ID, type));
	}

	public void printNodes() {
		for (GraphNode node : nodes) {
			System.out.println(node);
		}
	}

	public GraphNode getByID(int ID) {
		for (GraphNode node : nodes) {
			if (ID == node.ID) {
				return node;
			}
		}

		return null;
	}	
	
	public void printMatching(String pattern) {
		for (GraphNode node : nodes) {
			 if(node.getType() == pattern.charAt(0)) {
				 printMatchingRec(node, pattern, "" + node.getID(), 0);					 
			 }	
		}
	}
	
	private void printMatchingRec(GraphNode node, String pat, String cur, int i) {
		if(i >= pat.length() - 1) {
			System.out.println(cur);
			return;
		} 		
		
		for (int nextID : node.getConnections()) {
			GraphNode next = getByID(nextID);

			if (next.getType() == pat.charAt(i + 1)) {
				printMatchingRec(next, pat, cur + next.getID(), i + 1);
			}
		}
	}

	private class GraphNode {
		private final int ID;
		private final char type;
		private List<Integer> connections = new ArrayList<Integer>();

		public GraphNode(int ID, char type) {
			this.ID = ID;
			this.type = type;
		}

		public GraphNode(int ID, char type, int... connections) {
			this(ID, type);

			for (int curr : connections) {
				this.connections.add(curr);
			}
		}
		
		private char getType() {
			return type;
		}
		
		private int getID() {
			return ID;
		}
		
		private List<Integer> getConnections() {
			return connections;
		}
		
		private boolean isConnectedTo(int ID) {
			for (int connection : connections) {
				if (ID == connection) {
					return true;
				}
			}

			return false;
		}

		private void addConnection(int connectedID) {
			if (!isConnectedTo(connectedID)) {
				connections.add(connectedID);
			}
		}

		@Override
		public String toString() {
			return "ID " + ID +
				   " TYPE " + type +
				   " Connections " + connections.toString();
		}
	}
}

public class Q6 {
	public static void main(String[] args) {
		Graph g = new Graph();

		g.insert(1, 'A');
		g.insert(2, 'B', 1);
		g.insert(3, 'D', 2);
		g.insert(4, 'A');
		g.insert(5, 'C', 2);
		g.insert(6, 'B', 4, 5);
		g.insert(7, 'E', 4);
		g.insert(8, 'C', 6);

		g.updateConnections();

		g.printMatching("ABC");
	}
}
