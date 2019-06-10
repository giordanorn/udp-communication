package server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Server {
	private Map<String, ClientConnection> clients;
	private ServerSocket serverSocket;
	private int port;
	private final int maxConnections;
	private int currentConnectionCount;
	
	public Server(int port, int maxConnections) {
		this.port = port;
		this.maxConnections = maxConnections;
		this.currentConnectionCount = 0;
		this.clients = new ConcurrentHashMap<>();
	}
	
	public void broadcastMessageToAlmostAllClients(String msg, String clientNotToSend) {
		for(Map.Entry<String, ClientConnection> client : clients.entrySet()) {
			if(client.getKey().equals(clientNotToSend))
				continue;
			
			client.getValue().sendMessage(msg);
		}
	}
	
	public boolean isNameAvailable(String name, String address) {
		for(Map.Entry<String, ClientConnection> client : clients.entrySet()) {
			if(!client.getKey().equals(address)) {
				if(client.getValue().getClientName().equals(name))
					return false;
			}
		}
		
		return true;
	}
	
	public void start() throws IOException {
		serverSocket = new ServerSocket(port);
		
		while(true) {
			if(currentConnectionCount < maxConnections) {
				waitsForNewConnection();
			}
		}
	}
	
	private void waitsForNewConnection() throws IOException {
		Socket clientSocket = serverSocket.accept();
		System.out.println("A NEW CLIENT HAS CONNECTED TO THE SERVER");
		initNewConnection(clientSocket);
	}
	
	private void initNewConnection(Socket clientSocket) throws IOException {
		ClientConnection conn = new ClientConnection(clientSocket, this);
		clients.put(conn.getClientAddress(), conn);
		System.out.println("THE CLIENT WAS ADDED TO THE LIST OF CLIENTS");
		conn.start();
	}
	
	public void removeConnection(String address) {
		clients.remove(address);
	}	
}
