package jogadores;

import java.util.Scanner;

import velha.CampoPreenchido;
import velha.Velha;

public class UsuarioConsole implements Jogador {
	private Scanner scan;
	private boolean modoConsole;
	private int jogador;
	
	public UsuarioConsole()
	{
		modoConsole = false;
	}
	
	public UsuarioConsole(Scanner scan, int jogador)
	{
		modoConsole = true;
		this.jogador = jogador;
		this.scan = scan;
	}
	
	@Override
	public int fazerJogada(Velha velha, int index) {
		if(modoConsole)
		{
			do
			{
				System.out.println(velha.toString());
				System.out.print("index:");
				try {
					return velha.fazerJogada(scan.nextInt(), jogador);
				}catch(CampoPreenchido e)
				{
					System.out.println("Jogada Inválida");
				}
			}while(true);
		}
		else
		{
			return 3;
		}
	}

	@Override
	public int loop(Velha velha) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void reiniciar() {
		// TODO Auto-generated method stub
		
	}

}
