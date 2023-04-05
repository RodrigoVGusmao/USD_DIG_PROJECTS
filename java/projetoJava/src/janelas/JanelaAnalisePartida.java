package janelas;

import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

import bancoDeDados.Banco;
import bancoDeDados.HistoricoJogo;
import jogadores.Jogador;
import jogadores.RecordedPlayer;
import velha.Velha;

import org.eclipse.swt.widgets.List;

import java.sql.SQLException;
import java.util.Vector;

import org.eclipse.swt.SWT;
import org.eclipse.jface.viewers.ListViewer;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;

public class JanelaAnalisePartida extends Dialog {

	protected Object result;
	protected Shell shlHistricoDePartidas;

	/**
	 * Create the dialog.
	 * @param parent
	 * @param style
	 */
	public JanelaAnalisePartida(Shell parent, int style) {
		super(parent, style);
		setText("SWT Dialog");
	}

	/**
	 * Open the dialog.
	 * @return the result
	 */
	public Object open() {
		createContents();
		shlHistricoDePartidas.open();
		shlHistricoDePartidas.layout();
		Display display = getParent().getDisplay();
		while (!shlHistricoDePartidas.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
		return result;
	}

	/**
	 * Create contents of the dialog.
	 */
	private void createContents() {
		shlHistricoDePartidas = new Shell(getParent(), SWT.DIALOG_TRIM | SWT.APPLICATION_MODAL);
		shlHistricoDePartidas.setSize(450, 300);
		shlHistricoDePartidas.setText("Hist\u00F3rico de partidas");
		
		ListViewer listViewer = new ListViewer(shlHistricoDePartidas, SWT.BORDER | SWT.V_SCROLL);
		List list = listViewer.getList();
		list.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				String jogo = list.getSelection()[0];
				jogo = jogo.substring(jogo.indexOf('[') + 1, jogo.indexOf(']'));
				String[] arrjogo = retornarArray(jogo);
				Jogador j1 = new RecordedPlayer(arrjogo, 1, 1500);
				Jogador j2 = new RecordedPlayer(arrjogo, 2, 1500);
				Velha velha = new Velha();
				
				JanelaVelha janela = new JanelaVelha(velha, shlHistricoDePartidas, 0);
				janela.definirJogadores(j1, j2);
				janela.open();
			}
			@Override
			public void widgetDefaultSelected(SelectionEvent e) {
				String jogo = list.getSelection()[list.getSelectionIndex()];
				jogo = jogo.substring(jogo.indexOf('[') + 1, jogo.indexOf(']'));
				String[] arrjogo = retornarArray(jogo);
				Jogador j1 = new RecordedPlayer(arrjogo, 1, 1500);
				Jogador j2 = new RecordedPlayer(arrjogo, 2, 1500);
				Velha velha = new Velha();
				
				JanelaVelha janela = new JanelaVelha(velha, shlHistricoDePartidas, 0);
				janela.definirJogadores(j1, j2);
				janela.open();
			}
		});
		list.setBounds(0, 0, 434, 261);

		try {
			Banco banco = Banco.criarBanco();
			Vector<HistoricoJogo> data = banco.retornarHistoricoPartidas();
			for(int i = 0; i < data.size(); ++i)
				list.add(((HistoricoJogo)data.elementAt(i)).toString());
		} catch (SQLException e) {}
	}
	
	private String[] retornarArray(String str)
	{
		Vector<String> vec = new Vector<String>();
		int i = 0;
		while(true)
		{
			try {
				String temp = str.substring(i, str.indexOf(',', i+1));
				vec.add(temp.replaceAll("[^\\d]", ""));
			} catch(StringIndexOutOfBoundsException e)
			{
				vec.add(str.substring(i).replaceAll("[^\\d]", ""));
				break;
			}
			i = str.indexOf(',', i+1);
		}
		String[] arr = new String[vec.size()];
		vec.toArray(arr);
		
		return arr;
	}
}