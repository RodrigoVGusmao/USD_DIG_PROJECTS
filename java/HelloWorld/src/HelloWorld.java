import java.util.Scanner;

public class HelloWorld {

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		
		System.out.print("Insira o nome: ");
		String nome = input.nextLine();
		System.out.println(nome);
		
		input.close();
	}

}
