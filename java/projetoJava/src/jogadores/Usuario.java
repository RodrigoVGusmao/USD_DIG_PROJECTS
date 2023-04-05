package jogadores;

import velha.Velha;

public class Usuario implements Jogador {
	private int jogador;
	
	public Usuario(int jogador)
	{
		this.jogador = jogador;
	}
	
	@Override
	public int loop(Velha velha)
	{
		return 4;
	}
	
	@Override
	public int fazerJogada(Velha velha, int index) {
		return velha.fazerJogada(index, jogador);
	}

	@Override
	public void reiniciar() {
		return;
	}
}
