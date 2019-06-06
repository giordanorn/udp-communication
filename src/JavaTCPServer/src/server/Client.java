package server;

public class Client{
	private String name;
	private String address;
	
	public Client(String address) {
		this.address = address;
		this.name = null;
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getAddress() {
		return address;
	}
	public void setAddress(String address) {
		this.address = address;
	}
	public boolean hasName() {
		return this.name != null && !this.name.equals("");
	}
	
}