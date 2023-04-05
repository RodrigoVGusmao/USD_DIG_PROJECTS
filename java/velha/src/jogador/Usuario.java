package jogador;

import java.util.Scanner;

import velha.CampoPreenchido;
import velha.Velha;

public class Usuario implements Jogador {
	private Scanner scan;
	private boolean modoConsole;
	
	public Usuario()
	{
		modoConsole = false;
	}
	
	public Usuario(Scanner scan)
	{
		modoConsole = true;
		this.scan = scan;
	}
	
	public int fazerJogada(Velha velha, int jogador) {
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
					System.out.println("Jogada Inv�lida");
				}
			}while(true);
		}
		else
		{
			return 3;
		}
	}

}