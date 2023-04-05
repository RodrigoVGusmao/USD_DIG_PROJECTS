package jogador;

import java.util.Arrays;
import java.util.Collections;
import java.util.function.IntUnaryOperator;

import velha.Velha;

class possibilidade implements Comparable<possibilidade> {
	public int index;
	public int estado;
	public int pontuacao = 0;
	public possibilidade proximo[];
	
	public int compareTo(possibilidade o)
	{
		return pontuacao - o.pontuacao;
	}
}

public class Computador implements Jogador {
	private possibilidade[] bifurcacoes;
	private possibilidade atual;
	private double dificuldade;
	
	public Computador(double dificuldade)
	{
		this.dificuldade = dificuldade;
		bifurcacoes = new possibilidade[1];
		bifurcacoes[0] = new possibilidade();
		
		bifurcacoes[0].proximo = new possibilidade[9];
		for(int i = 0; i < 9; ++i)
			bifurcacoes[0].proximo[i] = new possibilidade();
		
		bifurcacoes[0].pontuacao = simularJogada(9, 2, 0, bifurcacoes[0].proximo);
		atual = bifurcacoes[0];
	}
	
	public String imprimirAnaliseCompleta()
	{
		return imprimirAnalise(bifurcacoes, 0, -1 >> 1);
	}
	
	public String imprimirAnalise(int profundidade)
	{
		return imprimirAnalise(bifurcacoes, 0, profundidade);
	}
	
	public int fazerJogada(Velha velha, int computador)
	{
		int[] campo = velha.RetornarCampo();
		int bitCampo = empacotarDados(campo, 2, (p1) -> {return p1;});
		acertarEstado(bitCampo);
		
		if(computador == 1)
			Arrays.sort(atual.proximo);
		else if(computador == 2)
			Arrays.sort(atual.proximo, Collections.reverseOrder());
		
		double escolha = Math.random();
		double somaCos = 0;
		for(int i = 0; i <= atual.proximo.length; ++i)
		{
			somaCos += dificuldade - dificuldade*Math.cos(((double)(i)/(double)(atual.proximo.length))*(Math.PI/2));
		}
		
		escolha = 1.0;
		int index = 0;
		for(double j = 0; index < atual.proximo.length-1; ++index)
		{
			j += (dificuldade - dificuldade*Math.cos(((double)(index)/(double)(atual.proximo.length))*(Math.PI/2)))/somaCos;
			if(j >= escolha)
				break;
		}
		
		return velha.fazerJogada(atual.proximo[index].index, computador);
	}
	
	//meteodos privados
	private int empacotarDados(int[] dados, int bitsPorDado, IntUnaryOperator func)
	{
		int empacotado = 0;
		if(32/(dados.length*bitsPorDado) < 1)
			throw new RuntimeException("Impossivel empacotar dado");
			
		for(int i = 0; i < dados.length; ++i)
		{
			empacotado |= func.applyAsInt(dados[i]) << i*bitsPorDado;
		}
		return empacotado;
	}
	
	//definir estado atual do campo para a IA
	private void acertarEstado(int estado)
	{
		int i = 0;
		while(estado != atual.estado)
		{
			if(i >= atual.proximo.length)
				throw new RuntimeException("N�o foi possivel encontrar possibilidade de campo");
			
			if((atual.proximo[i].estado & estado) == atual.proximo[i].estado)
			{
				atual = atual.proximo[i];
				i = 0;
			}
			else
				++i;
		}
	}
	
	//analisar estado do jogo (J1 ganhou = 1, J2 ganhou = -1, Empate = 0, Em andamento = 0)
	private int AnalisarEstado(int estado)
	{
		//teste horizontal
		int estadoTemp = estado;
		for(int i = 0; i < 3; ++i)
		{
			int resultado = estadoTemp & 0x3F;
			if(resultado == 0x15)
				return 1;
			if(resultado == 0x2A)
				return -1;
			estadoTemp >>= 6;
		}
		
		//teste vertical
		estadoTemp = estado;
		for(int i = 0; i < 3; ++i)
		{
			int resultado = estadoTemp & 0x30C3;
			if(resultado == 0x1041)
				return 1;
			if(resultado == 0x2082)
				return -1;
			estadoTemp >>= 2;
		}
		
		//teste digaonal esquerda para direita
		if((estado & 0x30303) == 0x10101)
			return 1;
		if((estado & 0x30303) == 0x20202)
			return -1;
		
		//teste digaonal direita para esquerda
		if((estado & 0x3330) == 0x1110)
			return 1;
		if((estado & 0x3330) == 0x2220)
			return -1;
		
		//sem vitoria;
		return 0;
	}
	
	//gerar arvore de possibilidades de jogo
	private int simularJogada(int nivelInicial, int jogador, int estadoInicial, possibilidade[] bifurcacao)
	{
		int pontuacao = 0;
		jogador ^= 3;
		for(int i = 0; i < bifurcacao.length; ++i)
		{
			int estado = estadoInicial;
			int j = 0;
			for(int k = 0; j < nivelInicial; ++j)
			{
				if(((estado >> 2*j) & 3) == 0)
					++k;
				if(k == i+1)
					break;
			}
			bifurcacao[i].index = j;
			bifurcacao[i].estado = estadoInicial | (jogador << (j*2));
			int resultado = AnalisarEstado(bifurcacao[i].estado)*524288;
			if(resultado == 0)
			{
				bifurcacao[i].proximo = new possibilidade[bifurcacao.length - 1];
				for(int k = 0; k < bifurcacao.length - 1; ++k)
				{
					bifurcacao[i].proximo[k] = new possibilidade();
				}
				bifurcacao[i].pontuacao += simularJogada(nivelInicial, jogador, bifurcacao[i].estado, bifurcacao[i].proximo)/32;
				pontuacao += bifurcacao[i].pontuacao;
			}
			else
			{
				bifurcacao[i].pontuacao = resultado;
				pontuacao += resultado;
			}
		}
		
		return pontuacao;
	}
	
	//imprimir arvore de possibilidades de jogo
	private String imprimirAnalise(possibilidade[] bifurcacao, int level, int profundidade)
	{
		String msg = "";
		if(bifurcacao == null)
			return "";
		for(int i = 0; i < bifurcacao.length; ++i)
		{
			for(int j = 0; j < level; ++j)
				msg += "\t";
			msg += bifurcacao[i].index + "; " + bifurcacao[i].estado +
					"; " + bifurcacao[i].proximo + "; " + bifurcacao[i].pontuacao + ";\n";
			if(profundidade > 0)
				msg += imprimirAnalise(bifurcacao[i].proximo, level + 1, profundidade - 1);
		}
		return msg;
	}
}