package questao1;

import javax.swing.JOptionPane;

public class Main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int maximo;
		try {
			maximo = Integer.parseInt(JOptionPane.showInputDialog("Digite o valor maximo do número aleatorio: (0-x)", "1"));
		}catch(Exception e)
		{
			JOptionPane.showMessageDialog(null, "ocorreu um erro durante a entrada de dados\n"
					+ "Atribuindo valor 1");
			maximo = 1;
		}
		
		NumeroAleatorio aleatorio = new NumeroAleatorio(maximo);
		JOptionPane.showMessageDialog(null, String.valueOf(aleatorio.gerarNumero()));
	}

}
