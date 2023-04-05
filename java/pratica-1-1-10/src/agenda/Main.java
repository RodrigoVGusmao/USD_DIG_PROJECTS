package agenda;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Random;

import javax.swing.*;

public class Main implements ActionListener {
	static JFrame frame = new JFrame("Agenda");
	static Agenda agenda = new Agenda();
	static JPanel janelaPrincipal = new JPanel(new GridLayout(0, 1));
	static JPanel botoes = new JPanel(new GridLayout(0, 3));
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		//janela principal
		JButton sairB = new JButton("sair");
		JButton adiconarContB = new JButton("adicionar contato...");
		JButton removerContB = new JButton("remover contato...");
		
		//ActionListeners
		sairB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				frame.setVisible(false);
				frame.dispose();
			}
		});
		
		adiconarContB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Random rand = new Random();
				
				JPanel adicionarContP = new JPanel(new GridLayout(0, 2));
				adicionarContP.add(new JLabel("Nome:"));
				JTextField nomeText = new JTextField(Integer.toString(rand.nextInt()));
				adicionarContP.add(nomeText);
				
				adicionarContP.add(new JLabel("Telefone:"));
				JTextField telefoneText = new JTextField(Integer.toString(rand.nextInt()));
				adicionarContP.add(telefoneText);
				
				adicionarContP.add(new JLabel("Email:"));
				JTextField emailText = new JTextField(Integer.toString(rand.nextInt()));
				adicionarContP.add(emailText);
				
				if(JOptionPane.showConfirmDialog(null, adicionarContP, "Adicionar Contato...", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION)
				{
					agenda.armazenaPessoa(nomeText.getText(), telefoneText.getText(), emailText.getText());
					
					janelaPrincipal.removeAll();
					janelaPrincipal.add(agenda.imprimeAgenda(2));
					janelaPrincipal.add(botoes);
					
					frame.pack();
					frame.invalidate();
					frame.validate();
					frame.repaint();
				}
			}
		});
		
		removerContB.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JPanel removerContP = new JPanel(new GridLayout(0, 2));
				removerContP.add(new JLabel("Nome:"));
				JTextField nomeText = new JTextField();
				removerContP.add(nomeText);
				
				if(JOptionPane.showConfirmDialog(null, removerContP, "Remover Contato...", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION)
				{
					agenda.removePessoa(nomeText.getText());
					
					janelaPrincipal.removeAll();
					janelaPrincipal.add(agenda.imprimeAgenda(2));
					janelaPrincipal.add(botoes);
					
					frame.pack();
					frame.invalidate();
					frame.validate();
					frame.repaint();
				}
			}
		});
		
		botoes.add(sairB);
		botoes.add(adiconarContB);
		botoes.add(removerContB);
		
		janelaPrincipal.add(agenda.imprimeAgenda(2));
		janelaPrincipal.add(botoes);
		
		frame.add(janelaPrincipal);
		frame.pack();
		frame.setVisible(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		
	}

}
