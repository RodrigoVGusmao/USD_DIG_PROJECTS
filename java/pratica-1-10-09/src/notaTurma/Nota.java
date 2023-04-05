package notaTurma;

public class Nota {
	private String materia;
	private float nota;
	
	public Nota(String materia, float nota)
	{
		this.materia = materia;
		this.nota = nota;
	}
	
	public Nota() 
	{
		this.materia = null;
		this.nota = 0.f;
	}
	
	public String getMateria() {
		return materia;
	}
	
	public void setMateria(String materia) {
		this.materia = materia;
	}
	
	public float getNota() {
		return nota;
	}
	
	public void setNota(float nota) {
		this.nota = nota;
	}

}
