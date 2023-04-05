package jogadores;

import velha.Velha;

public class RecordedPlayer implements Jogador {

	private int jogador;
	private int delay;
	private String[] jogo;
	
	public RecordedPlayer(String[] jogo, int jogador, int delay)
	{
		this.jogador = jogador;
		this.jogo = jogo;
		this.delay = delay;
	}
	
	@Override
	public int loop(Velha velha) {
		// TODO Auto-generated method stub
		int turno = velha.camposPreenchidos();
		int jogada = Integer.parseInt(jogo[turno], 4);
		if(turno > 0)
		{
			jogada ^= Integer.parseInt(jogo[turno-1], 4);
		}
		try {
			Thread.sleep(delay);
		} catch (InterruptedException e) {}
		return fazerJogada(velha, retornarIndex(jogada));
	}

	@Override
	public int fazerJogada(Velha velha, int index) {
		// TODO Auto-generated method stub
		return velha.fazerJogada(index, jogador);
	}

	@Override
	public void reiniciar() {
		// TODO Auto-generated method stub
		
	}

	private int retornarIndex(int jogada)
	{
		int temp = jogada;
		int i;
		for(i = 1; temp != 0; ++i)
		{
			temp = jogada >> (i*2);
		}
		return i-2;
	}
}