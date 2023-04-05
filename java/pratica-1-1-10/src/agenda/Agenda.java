package agenda;

import java.awt.GridLayout;
import java.util.ArrayList;

import javax.swing.*;

public class Agenda {
	private ArrayList<Contato> Contatos = new ArrayList<Contato>();
	
	public Agenda() {}
	public void armazenaPessoa(String nome, String telefone, String email) {
		Contatos.add(new Contato(nome, telefone, email));
	}
	public void removePessoa(String nome) {
		Contatos.removeIf(c -> (c.getNome().compareTo(nome) == 0));
	}
	public int buscaPessoa(String nome) throws Exception {
		
		for(int i = 0; i < Contatos.size(); ++i)
		{
			Contato temp = Contatos.get(i);
			if(temp.getNome().compareTo(nome) == 0)
				return i;
		}
		
		throw new Exception("Não existe " + nome + " na agenda");
	}
	
	@Override
	public String toString()
	{
		String msg = null;
		for(int i = 0; i < Contatos.size(); ++i)
		{
			msg = Contatos.get(i).toString() + "\n";
		}
		
		return msg;
	}
	
	public JPanel imprimeAgenda(int colunas) {
		JPanel panel = new JPanel(new GridLayout(0, colunas));
		for(int i = 0; i < Contatos.size(); ++i)
		{
			panel.add(new JLabel(Contatos.get(i).toString() + "     "));
		}
		
		return panel;
	}
	
	public String imprimePessoa(int index)
	{
		return Contatos.get(index).toString();
	}
}
