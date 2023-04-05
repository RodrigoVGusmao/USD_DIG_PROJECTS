package questao1;

import java.lang.Math;

public class NumeroAleatorio {
	private int numeroMax = 1;
	
	NumeroAleatorio(int numeroMax)
	{
		this.numeroMax = numeroMax;
	}
	
	public int gerarNumero()
	{
		return (int)(Math.random()*numeroMax);
	}
}
