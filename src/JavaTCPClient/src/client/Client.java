package client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;

public class Client {
	private String name;
	private Socket clientSocket;
	private PrintWriter out;
	private BufferedReader in;
	private String ipServer;
	private int portServer;
	
	public Client(String ipServer, int portServer, String name) throws IOException {
		clientSocket = new Socket(ipServer, portServer);
		out = new PrintWriter(clientSocket.getOutputStream(), true);
		in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		this.name = name;
		this.ipServer = ipServer;
		this.portServer = portServer;
	}
	
	public void init() throws IOException {
		new Thread() {
			@Override
			public void run() {
				String serverMessage;
				try {
					while ((serverMessage = in.readLine()) != null) {
						System.out.println(serverMessage);
					}
				}
				
				catch (SocketException e) {
					// socket was closed
				}
				
				catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
		}.start();

		System.out.println("Conectado ao servidor " + ipServer + " na porta " + portServer);
		out.println(name);
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		String msgToSend;
		
		while((msgToSend = reader.readLine())!= null) {
			out.println(msgToSend);
			
			if(msgToSend.equals("sair")) {
				stopConnection();
				break;
			}
		}
	}
	
	public void stopConnection() throws IOException {
		out.close();
		in.close();
		clientSocket.close();
	}
}
