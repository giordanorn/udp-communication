package main;

import java.io.IOException;

import server.Server;

public class Main {
	public static void main(String[] args) {
		Server server;
		
		if(args.length == 1)
			server = new Server(Integer.parseInt(args[0]), 10);
		else
			server = new Server(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
		
		try {
			server.start();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
