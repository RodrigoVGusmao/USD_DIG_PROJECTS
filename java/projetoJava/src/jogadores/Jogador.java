package jogadores;

import velha.Velha;

public interface Jogador {
	public int loop(Velha velha);
	public int fazerJogada(Velha velha, int index);
	public void reiniciar();
}
