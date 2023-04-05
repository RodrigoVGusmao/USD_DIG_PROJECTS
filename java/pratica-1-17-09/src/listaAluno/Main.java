package listaAluno;

import java.awt.GridLayout;
import java.util.ArrayList;

import javax.swing.*;

public class Main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final int alunos = 3;
		final int fieldsPerObject = 3;
		final int seed = 123456789;
		
		//input
		JPanel panel = new JPanel(new GridLayout(0, 3));
		JTextField field[] = new JTextField[alunos*fieldsPerObject];
		//panel.add(new JLabel("Notas"));
		for(int i = 0; i < alunos; ++i)
		{
			panel.add(new JLabel("Aluno  " + i));
			panel.add(new JLabel("Matricula" + i));
			panel.add(new JLabel("Email" + i));
			
			field[i*fieldsPerObject] = new JTextField("Aluno " + i);
			panel.add(field[i*fieldsPerObject]);
			
			field[i*fieldsPerObject+1] = new JTextField(String.valueOf(seed % (i+1)*seed));
			panel.add(field[i*fieldsPerObject+1]);
			
			field[i*fieldsPerObject+2] = new JTextField("E-mail" + i + "@elect.net");
			panel.add(field[i*fieldsPerObject+2]);
		}
		
		if(JOptionPane.showConfirmDialog(null, panel, "Alunos", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION)
		{
			//adicionando dados a ArrayList
			ArrayList<Aluno> turma = new ArrayList<Aluno>();
			for(int i = 0; i < alunos; ++i)
			{
				turma.add(new Aluno(field[i*fieldsPerObject].getText(), Integer.parseInt(field[i*fieldsPerObject+1].getText()), field[i*fieldsPerObject+2].getText()));
			}
			
			//ordem
			JPanel panelSort = new JPanel(new GridLayout(0, 3));
			JComboBox<String> sort = new JComboBox<String>();
			sort.addItem("nome");
			sort.addItem("matricula");
			sort.addItem("email");
			sort.addItem("nenhum");
			panelSort.add(sort);
			
			if(JOptionPane.showConfirmDialog(null, panelSort, "Ordem", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION)
			{
				//ordem
				switch(sort.getSelectedIndex())
				{
				case 0:
					turma.sort((a, b) -> {return a.getNome().compareToIgnoreCase(b.getNome()) > 0 ? 1 : 
												a.getNome().compareToIgnoreCase(b.getNome()) < 0 ? -1 : 0;});
					break;
				case 1:
					turma.sort((a, b) -> {return a.getMatricula() > b.getMatricula() ? 1 : 
												a.getMatricula() < b.getMatricula() ? -1 : 0;});
					break;
				case 2:
					turma.sort((a, b) -> {return a.getEmail().compareToIgnoreCase(b.getEmail()) > 0 ? 1 : 
												a.getEmail().compareToIgnoreCase(b.getEmail()) < 0 ? -1 : 0;});
					break;
				}
				
				//processamento
				JPanel panelResult = new JPanel(new GridLayout(0, 3));
				turma.forEach(aluno -> {panelResult.add(new JLabel(aluno.getNome()));
										panelResult.add(new JLabel(String.valueOf(aluno.getMatricula())));
										panelResult.add(new JLabel(aluno.getEmail()));});
				
				JOptionPane.showMessageDialog(null, panelResult, "Alunos", JOptionPane.PLAIN_MESSAGE);
			}
		}
	}

}
