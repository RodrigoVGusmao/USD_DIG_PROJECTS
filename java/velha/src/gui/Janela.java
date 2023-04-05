package gui;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

import velha.Velha;

public class Janela implements ActionListener{

	public JFrame janela;
	
	public Janela(Velha jogo)
	{
		janela = new JFrame("Velha");	
		JPanel velha = new JPanel(new GridLayout(0, 1)) {
			private static final long serialVersionUID = 1L;

			@Override
			public void paintComponent(Graphics g) {
				super.paintComponent(g);
				int[] estado = jogo.RetornarCampo();
				
				int w = getWidth();
				int h = getHeight();
				
				//linhas verticais
				g.drawLine((int)(0.1*w + w*0.8/3), (int)(h*0.1), (int)(0.1*w + w*0.8/3), (int)(h*0.9));
				g.drawLine((int)(0.1*w + 2*w*0.8/3), (int)(h*0.1), (int)(0.1*w + 2*w*0.8/3), (int)(h*0.9));
				
				//linhas horizontais
				g.drawLine((int)(w*0.1), (int)(0.1*h + h*0.8/3), (int)(w*0.9), (int)(0.1*h + h*0.8/3));
				g.drawLine((int)(w*0.1), (int)(0.1*h + 2*h*0.8/3), (int)(w*0.9), (int)(0.1*h + 2*h*0.8/3));
				
				for(int i = 0; i < estado.length; ++i)
				{
					if(estado[i] == 1)
					{
						g.drawLine((2*w/15 + 4*(i%3)*w/15), (2*h/15 + 4*(i/3)*h/15),
								(1*w/15 + 4*((i%3)+1)*w/15), (1*h/15 + 4*((i/3)+1)*h/15));
						g.drawLine((1*w/15 + 4*((i%3)+1)*w/15), (2*h/15 + 4*(i/3)*h/15),
								(2*w/15 + 4*(i%3)*w/15), (1*h/15 + 4*((i/3)+1)*h/15));
					}
					if(estado[i] == 2)
					{
						g.drawOval((2*w/15 + 4*(i%3)*w/15), (2*h/15 + 4*(i/3)*h/15), w/5, h/5);
					}
				}
			}
		};
		
		velha.setPreferredSize(new Dimension(500, 500));
		
		JPanel analise = new JPanel(new GridLayout(0, 3));
		
		janela.add(velha);
		janela.pack();
		janela.setVisible(true);
	}
	
	public void repaintFrame()
	{
		janela.invalidate();
		janela.validate();
		janela.repaint();
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		
	}

}