package server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientConnection {
	private Client client;
	private Server server;
	private ClientThread clientThread;
	private PrintWriter out;

	public ClientConnection(Socket clientSocket, Server server) throws IOException {
		this.out = new PrintWriter(clientSocket.getOutputStream(), true);
		this.server = server;
		this.client = new Client(clientSocket.getInetAddress().getHostAddress());
		this.clientThread = new ClientThread(clientSocket);
	}

	public void start() {
		clientThread.start();
	}

	public void sendMessage(String msg) {
		out.println(msg);
	}

	public String getClientName() {
		return client.getName();
	}

	public String getClientAddress() {
		return client.getAddress();
	}

	private class ClientThread extends Thread {
		private BufferedReader in;
		private Socket clientSocket;

		public ClientThread(Socket clientSocket) {
			this.clientSocket = clientSocket;
		}

		public void closeConnection() throws IOException {
			this.in.close();
			out.close();
			clientSocket.close();
			server.removeConnection(client.getAddress());
		}

		public void run() {
			try {
				this.in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
				String receivedMessage;

				while ((receivedMessage = in.readLine()) != null) {
					
					if (!client.hasName()) {
						if (!server.isNameAvailable(receivedMessage, client.getAddress())) {
							sendMessage("ja tem ume amiguinhe com esse nome. please choose another one.");
						}
						
						else {
							client.setName(receivedMessage);
							server.broadcastMessageToAlmostAllClients(client.getName() + " entrou na sala.",
									client.getAddress());
							System.out.println(client.getName() + " entrou na sala");
						}
					}

					else if (receivedMessage.equals("sair")) {
						closeConnection();
						break;
					}

					else {
						server.broadcastMessageToAlmostAllClients(client.getName() + " disse: " + receivedMessage,
								null);
						System.out.println(client.getName() + " disse: " + receivedMessage);
					}
				}
			}

			catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}
