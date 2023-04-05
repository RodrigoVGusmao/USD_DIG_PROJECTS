package somaPar;

import javax.swing.JOptionPane;

public class somaPar {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int maximo, minimo;
		try {
			maximo = Integer.parseInt(JOptionPane.showInputDialog("Digite o valor maximo do número da soma:", "2"));
			minimo = Integer.parseInt(JOptionPane.showInputDialog("Digite o valor minimo do número da soma:", "1"));
		}catch(Exception e)
		{
			JOptionPane.showMessageDialog(null, "ocorreu um erro durante a entrada de dados\n"
					+ "Atribuindo valor 1");
			maximo = 1;
			minimo = 2;
		}
		
		int temp = maximo;
		maximo = (maximo > minimo) ? maximo & (~1) : minimo & (~1);
		minimo = ((maximo & (~1)) == (minimo & (~1))) ? temp & (~1) : minimo & (~1);
		
		JOptionPane.showMessageDialog(null, "Resultado entre " + minimo + " e " + maximo + ": "
		+ );
	}

}
