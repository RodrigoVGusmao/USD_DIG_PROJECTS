import java.util.Scanner;

public class JogoSimples {
	public static String converteTempo(int segundos)
	{
		String tempo;
		if(segundos < 0)
			tempo = "-";
		else
			tempo = "";
		
		int resto = Math.abs(segundos)%3600;
		tempo += Integer.toString(Math.abs(segundos)/3600) + ":" + Integer.toString(resto/60) + ":";
		resto %= 60;
		tempo += resto;
		
		return tempo;
	}
	
	public static int somaDigitos(int input)
	{
		if(input != 0)
			return input % 10 + somaDigitos(input/10);
		else
			return 0;
		
	}
	
	public static void main(String[] args) {
		// Crie uma instância da classe jogoSimples.
		// TODO: Seu código vai aqui
		
		Scanner sc = new Scanner(System.in);
		
		System.out.println("1- converteTempo;");
		System.out.println("2- somaDígitos:");
		System.out.print("selecione o modo (1 ou 2):");
		switch(sc.nextInt())
		{
		case 1:
			System.out.println("\nInsira tempo em segundos:");
			System.out.println(converteTempo(sc.nextInt()));
			break;
		case 2:
			System.out.println("\nInsira o número:");
			System.out.println(somaDigitos(sc.nextInt()));
			break;
		default:
			System.out.println("entrada inválida!");
		}
		
		sc.close();
	}
}
