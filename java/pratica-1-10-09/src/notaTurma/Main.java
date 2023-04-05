package notaTurma;

import java.awt.GridLayout;
import javax.swing.*;

public class Main {

	public static void main(String[] args) {
		try {
			final int materias = 10;
			
			String aluno = new String();
			aluno = JOptionPane.showInputDialog("informe o nome do aluno:");
			
			//input
			JPanel panel = new JPanel(new GridLayout(0, 2));
			JTextField field[] = new JTextField[materias*2];
			//panel.add(new JLabel("Notas"));
			for(int i = 0; i < materias; ++i)
			{
				panel.add(new JLabel("Materia " + i));
				panel.add(new JLabel("Nota " + i));
				
				field[i*2] = new JTextField("Materia" + i);
				panel.add(field[i*2]);
				
				field[i*2+1] = new JTextField(String.valueOf(10 - i));
				panel.add(field[i*2+1]);
			}
			
			if(JOptionPane.showConfirmDialog(null, panel, "Notas", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION)
			{
				//processamento
				//adicionando ao vetor
				Nota notas[] = new Nota[materias];
				for(int i = 0; i < materias; ++i)
				{
					notas[i] = new Nota(field[i*2].getText(), Float.parseFloat(field[i*2 + 1].getText()));
				}
				
				//processando informações
				JPanel panelResult = new JPanel(new GridLayout(0, 1));
				panelResult.add(new JLabel(aluno));
				float somaNota = 0;
				Nota notaMax = notas[0];
				Nota notaMin = notas[0];
				for(int i = 0; i < materias; ++i)
				{
					notaMax = (notas[i].getNota() > notaMax.getNota()) ? notas[i] : notaMax;
					notaMin = (notas[i].getNota() < notaMin.getNota()) ? notas[i] : notaMin;
					somaNota += notas[i].getNota();
					if(notas[i].getNota() > 10 || notas[i].getNota() < 0)
						throw new Exception("dado invalido"); 
					panelResult.add(new JLabel(notas[i].getMateria() + ' ' + notas[i].getNota()));
				}
				panelResult.add(new JLabel("media: " + somaNota/materias));
				panelResult.add(new JLabel("maior nota: " + notaMax.getNota() + " - " + notaMax.getMateria()));
				panelResult.add(new JLabel("menor nota: " + notaMin.getNota() + " - " + notaMin.getMateria()));
				
				JOptionPane.showMessageDialog(null, panelResult, "Notas", JOptionPane.PLAIN_MESSAGE);
			}
			
		}catch(Exception e)
		{
			JOptionPane.showMessageDialog(null, "ocorreu um erro durante a entrada de dados:\n" + e.getMessage());
		}
	}

}
