package listaAluno;

public class Aluno {
	private String nome;
	private int matricula;
	private String email;
	
	Aluno()
	{
		nome = null;
		matricula = 0;
		email = null;
	}
	
	Aluno(String nome, int matricula, String email)
	{
		this.nome = nome;
		this.matricula = matricula;
		this.email = email;
	}
	
	//getters and setters
	public int getMatricula() {
		return matricula;
	}

	public void setMatricula(int matricula) {
		this.matricula = matricula;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}
	
	public String getNome()
	{
		return nome;
	}
	
}
