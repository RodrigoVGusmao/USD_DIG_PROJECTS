package janelas;

import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.widgets.Label;

import jogadores.Jogador;
import velha.CampoPreenchido;
import velha.Velha;

public class MouseAdapterVelha extends MouseAdapter {
	JanelaVelha janela;
	Jogador jogador;
	Velha velha;
	int index;
	
	public MouseAdapterVelha(JanelaVelha janela, Velha velha, int index)
	{
		super();
		this.janela = janela;
		this.velha = velha;
		this.index = index;
	}
	
	@Override
	public void mouseUp(MouseEvent e)
	{
		jogador = janela.retornarJogadorAtual();
		janela.trocarJogador();
		System.out.println(jogador);
		try {
			int estado = jogador.fazerJogada(velha, index);
			Label lbl = ((Label) e.getSource());
			lbl.redraw();
			lbl.update();
			janela.testarEstado(estado);
		}catch(CampoPreenchido e1)
		{
			return;
		}
	}
}
