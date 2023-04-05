package janelas;

import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;

import bancoDeDados.Banco;

import org.eclipse.swt.widgets.Button;

import java.sql.Connection;
import java.sql.SQLException;

import javax.swing.JOptionPane;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;

public class JanelaAdministrarConta extends Dialog {

	protected Object result;
	protected Shell shlAdministrarConta;

	/**
	 * Create the dialog.
	 * @param parent
	 * @param style
	 */
	public JanelaAdministrarConta(Shell parent, int style) {
		super(parent, style);
		setText("SWT Dialog");
	}

	/**
	 * Open the dialog.
	 * @return the result
	 */
	public Object open() {
		createContents();
		shlAdministrarConta.open();
		shlAdministrarConta.layout();
		Display display = getParent().getDisplay();
		while (!shlAdministrarConta.isDisposed()) {
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
		shlAdministrarConta = new Shell(getParent(), SWT.DIALOG_TRIM | SWT.APPLICATION_MODAL);
		shlAdministrarConta.setSize(152, 116);
		shlAdministrarConta.setText("Administrar conta");
		
		Button btnDeletarConta = new Button(shlAdministrarConta, SWT.NONE);
		btnDeletarConta.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				Banco banco;
				try {
					banco = Banco.criarBanco();
				} catch (SQLException e1) {return;}
				int ID = banco.retornarIdUsuario();
				JanelaLogin autentica��o = new JanelaLogin(shlAdministrarConta, 0);
				try {
					((Connection) autentica��o.open()).close();
				} catch (SQLException e2) {}
				
				if(ID != banco.retornarIdUsuario())
				{
					JOptionPane.showMessageDialog(null, "LoginIncorreto", "erro", JOptionPane.OK_OPTION);
					try {
						banco.restoreConnection(ID);
					} catch (SQLException e1) {}
					shlAdministrarConta.close();
				}
				
				try {
					banco.deletarUsuario(banco.retornarNomeUsuario());
				} catch (SQLException e1) {
					JOptionPane.showMessageDialog(null, "N�o foi possivel deletar usuario!" + "\nErro: " + e1.getMessage(),
							"erro", JOptionPane.OK_OPTION);
					try {
						banco.restoreConnection(ID);
					} catch (SQLException e2) {}
				}
				JanelaMain.sairConta();
				shlAdministrarConta.close();
			}
		});
		btnDeletarConta.setBounds(10, 10, 116, 25);
		btnDeletarConta.setText("Deletar Conta");
		
		Button btnApagarHistrico = new Button(shlAdministrarConta, SWT.NONE);
		btnApagarHistrico.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				Banco banco;
				try {
					banco = Banco.criarBanco();
				} catch (SQLException e1) {return;}
				int ID = banco.retornarIdUsuario();
				JanelaLogin autentica��o = new JanelaLogin(shlAdministrarConta, 0);
				try {
					((Connection) autentica��o.open()).close();
				} catch (SQLException e2) {}
				
				if(ID != banco.retornarIdUsuario())
				{
					JOptionPane.showMessageDialog(null, "LoginIncorreto", "erro", JOptionPane.OK_OPTION);
					try {
						banco.restoreConnection(ID);
					} catch (SQLException e1) {}
					shlAdministrarConta.close();
				}
				
				try {
					banco.apagarHistorico(banco.retornarIdUsuario());
				} catch (SQLException e1) {
					JOptionPane.showMessageDialog(null, "N�o foi possivel deletar hist�rico de jogos do usuario!"
				+ "\nErro: " + e1.getMessage(), "erro", JOptionPane.OK_OPTION);
				}
				try {
					JanelaMain.atualizarStatus();
				}catch(SQLException e1) {}
				shlAdministrarConta.close();
			}
		});
		btnApagarHistrico.setBounds(10, 41, 116, 25);
		btnApagarHistrico.setText("Apagar hist\u00F3rico");

	}

}