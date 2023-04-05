package empregado;

import java.util.Scanner;

public class Empregado {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner scan = new Scanner(System.in);
		System.out.println("digite o nome do empregado:");
		String nome = scan.nextLine();
		
		System.out.println("digite o salário do empregado:");
		float salario = scan.nextFloat();
		
		System.out.println("digite o total de dinheiro em vendas do empregado:");
		float vendas = scan.nextFloat();
		
		System.out.println("empregado:\t\t\t"+nome+"\nsalário:\t\t\t"+salario+"\nsalario total:\t\t\t"+(salario+vendas*0.15));
		
		scan.close();
	}

}
