package main;

import java.io.IOException;

import client.Client;

public class Main {
	public static void printHelp() {
		System.out.println("usage: java -jar client.jar <server_address> <server_port> <your_nickname>");
		
	}
	
	public static void main(String[] args) {
		try {
			Client client = new Client(args[0], Integer.parseInt(args[1]), args[2]);
			client.init();
			
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch(ArrayIndexOutOfBoundsException | NumberFormatException e) {
			printHelp();
		}
	}

}
