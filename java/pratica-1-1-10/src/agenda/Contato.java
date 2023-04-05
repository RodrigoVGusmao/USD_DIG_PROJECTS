package agenda;

public class Contato {
	private String nome;
	private String telefone;
	private String email;
	
	public Contato() {}
	public Contato(String nome, String telefone, String email)
	{
		setContato(nome, telefone, email);
	}
	
	@Override
	public String toString()
	{
		return nome + ", " + telefone + ", " + email + ";";
	}
	
	//getters and setters
	public void setContato(String nome, String telefone, String email)
	{
		this.nome = nome;
		this.telefone = telefone;
		this.email = email;
	}
	public String getNome() {
		return nome;
	}
	public void setNome(String nome) {
		this.nome = nome;
	}
	public String getTelefone() {
		return telefone;
	}
	public void setTelefone(String telefone) {
		this.telefone = telefone;
	}
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	
}
