package janelas;

import java.sql.SQLException;

import javax.swing.JOptionPane;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.wb.swt.SWTResourceManager;

import bancoDeDados.Banco;

public class JanelaCadastro extends Dialog {

	protected Object result;
	protected Shell shlCadastro;
	protected Text text;
	protected Text text_1;
	protected Text text_2;
	protected Label lblLogin;
	protected Label lblSenha;
	protected Label lblConfirmarSenha;
	protected Label lblUsurioJExistente;
	protected Label lblSenhasDiferentes;
	protected Button btnNewButton;
	protected Button btnCancelar;

	/**
	 * Create the dialog.
	 * @param parent
	 * @param style
	 */
	public JanelaCadastro(Shell parent, int style) {
		super(parent, style);
		setText("SWT Dialog");
	}

	/**
	 * Open the dialog.
	 * @return the result
	 */
	public Object open() {
		createContents();
		shlCadastro.open();
		shlCadastro.layout();
		Display display = getParent().getDisplay();
		while (!shlCadastro.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
		return result;
	}

	/**
	 * Create contents of the dialog.
	 */
	protected void createContents() {
		shlCadastro = new Shell(getParent(), SWT.DIALOG_TRIM | SWT.PRIMARY_MODAL);
		shlCadastro.setSize(242, 285);
		shlCadastro.setText("cadastro");
		
		lblLogin = new Label(shlCadastro, SWT.NONE);
		lblLogin.setBounds(42, 20, 55, 15);
		lblLogin.setText("Usuário");
		
		lblSenha = new Label(shlCadastro, SWT.NONE);
		lblSenha.setBounds(42, 78, 55, 15);
		lblSenha.setText("Senha");
		
		text = new Text(shlCadastro, SWT.BORDER);
		text.setBounds(42, 41, 152, 21);
		
		text_1 = new Text(shlCadastro, SWT.BORDER | SWT.PASSWORD);
		text_1.setBounds(42, 99, 152, 21);
		
		lblConfirmarSenha = new Label(shlCadastro, SWT.NONE);
		lblConfirmarSenha.setBounds(42, 136, 89, 15);
		lblConfirmarSenha.setText("Confirmar senha");
		
		text_2 = new Text(shlCadastro, SWT.BORDER | SWT.PASSWORD);
		text_2.setBounds(42, 157, 152, 21);
		
		btnNewButton = new Button(shlCadastro, SWT.NONE);
		btnNewButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				if(!senhaIgual())
				{
					lblSenhasDiferentes.setVisible(true);
					return;
				}
				Banco banco;
				try {
					banco = Banco.criarBanco();
				} catch (SQLException e1) {
					// TODO Auto-generated catch block
					JOptionPane.showMessageDialog(null, "Não foi possivel fazer cadastro!" + "\nErro: " + e1.getMessage(),
							"erro", JOptionPane.OK_OPTION);
					return;
				}
				try {
					banco.criarUsuario(text.getText(), text_1.getText());
				} catch (SQLException e1) {
					// TODO Auto-generated catch block
					lblUsurioJExistente.setVisible(true);
				}
				try {
					banco = Banco.criarBanco();
				} catch (SQLException e1) {
					// TODO Auto-generated catch block
					JOptionPane.showMessageDialog(null, "Erro ao completar cadastro!" + "\nErro: " + e1.getMessage(),
							"erro", JOptionPane.OK_OPTION);
					return;
				}
				lblUsurioJExistente.setVisible(false);
				lblSenhasDiferentes.setVisible(false);
				shlCadastro.close();
			}
		});
		btnNewButton.setBounds(140, 210, 75, 25);
		btnNewButton.setText("Cadastrar");
		
		btnCancelar = new Button(shlCadastro, SWT.NONE);
		btnCancelar.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				shlCadastro.close();
			}
		});
		btnCancelar.setText("Cancelar");
		btnCancelar.setBounds(22, 210, 75, 25);
		
		lblUsurioJExistente = new Label(shlCadastro, SWT.NONE);
		lblUsurioJExistente.setForeground(SWTResourceManager.getColor(SWT.COLOR_RED));
		lblUsurioJExistente.setBounds(42, 184, 152, 15);
		lblUsurioJExistente.setText("Usu\u00E1rio j\u00E1 existente");
		lblUsurioJExistente.setVisible(false);
		
		lblSenhasDiferentes = new Label(shlCadastro, SWT.NONE);
		lblSenhasDiferentes.setForeground(SWTResourceManager.getColor(SWT.COLOR_RED));
		lblSenhasDiferentes.setBounds(42, 184, 152, 15);
		lblSenhasDiferentes.setText("Senhas diferentes");
		lblSenhasDiferentes.setVisible(false);
	}
	
	private boolean senhaIgual()
	{
		if(text_1.getText().equals(text_2.getText()))
			return true;
		return false;
	}
}
