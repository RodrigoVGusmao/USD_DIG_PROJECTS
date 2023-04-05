package bancoDeDados;

import java.sql.*;
import java.util.Vector;

import javax.swing.JOptionPane;

public final class Banco {
	private static Banco instance;
	private static final String hostName = "jdbc:mysql://localhost:3306/";
	private static final String banco = "bd_java_velha";
	private static final String adminUser = "root";
	private static final String psswd = "pssw";
	
	private int idUsuario;
	private String usuario;
	private int partidas;
	private int partidasGanhas;
	private int partidasPerdidas;
	private int partidasEmpatadas;
	private int partidasAbandonadas;
	
	public static Banco criarBanco() throws SQLException
	{
		if(instance == null)
		{
			instance = new Banco();
		}
		return instance;
	}
	
	private Banco() throws SQLException
	{
		Connection adminCon = null;
		
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			JOptionPane.showMessageDialog(null, "N�o foi possivel carregar driver!", "erro", JOptionPane.OK_OPTION);
		}
		
		while(true)
		{
			try {
				adminCon = createAdminConnection();
				break;
			} catch(SQLException e)
			{
				int result = JOptionPane.showConfirmDialog(null, "N�o foi possivel conectar ao banco de dados!" +
						" Deseja tentar novamente?" + "\nerror: " + e.getMessage(),
						"erro", JOptionPane.YES_NO_OPTION);
				if(result == JOptionPane.NO_OPTION)
				{
					throw e;
				}
			}
		}
		
		tentarComando(adminCon, "CREATE DATABASE IF NOT EXISTS " + banco, "N�o foi possivel criar banco de dados!", 3);
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		tentarComando(adminCon, "CREATE TABLE IF NOT EXISTS user_info (userId INT PRIMARY KEY AUTO_INCREMENT,"
				+ " user VARCHAR(50), games INT, wins INT, loses INT, draws INT, abandoned INT)",
				"N�o foi possivel criar tabela 'user' no banco de dados!", 3);
		tentarComando(adminCon, "CREATE TABLE IF NOT EXISTS game_history (gameId INT PRIMARY KEY AUTO_INCREMENT,"
				+ " userId INT, game JSON)", "N�o foi possivel criar tabela 'game' no banco de dados!", 3);
		tentarComando(adminCon, "CREATE ROLE IF NOT EXISTS tic_tac_toe_user", "N�o foi possivel criar fun��o 'user' no banco de dados!", 3);
		tentarComando(adminCon, "CREATE OR REPLACE VIEW user_view AS "
				+ "SELECT * FROM user_info WHERE user_info.userId = 0 GROUP BY user_info.userId",  
				"N�o foi possivel recriar view!", 3);
		
		adminCon.close();
	}
	
	public void criarUsuario(String user, String psswd) throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		tentarComando(adminCon, "CREATE USER \'" + user + "\' IDENTIFIED BY \'" + psswd + 
				"\' DEFAULT ROLE tic_tac_toe_user", "N�o foi possivel criar usu�rio!", 3);
		tentarComando(adminCon, "INSERT INTO user_info (user, games, wins, loses, draws, abandoned) "
				+ "VALUES (\'"+ user +"\', 0, 0, 0, 0, 0)", "N�o foi possivel alterar tabela", 3);
		adminCon.close();
	}
	
	public void alterarSenha(String user, String psswd) throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		tentarComando(adminCon, "ALTER USER \'" + user + "\' IDENTIFIED BY \'" + psswd, "N�o foi possivel criar usu�rio!", 3);
		adminCon.close();
	}
	
	public void deletarUsuario(String user) throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		tentarComando(adminCon, "DROP USER \'" + user + "\'", "N�o foi possivel deletar usu�rio!", 3);
		tentarComando(adminCon, "DELETE FROM user_info WHERE user_info.user = \'"+ user +"\'",
				"N�o foi possivel alterar tabela", 3);
		adminCon.close();
	}
	
	public void inserirJogo(int resultado, String jogo) throws SQLException
	{
		if(retornarInfoUsuario() == true)
		{
			Connection adminCon = createAdminConnection();
			tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
			tentarComando(adminCon, "INSERT INTO game_history (userId, game) "
					+ "VALUES ("+ idUsuario +", \'{ \"vitoria\": \"" + resultado
					+ "\", \"jogo\": [" + jogo + "]}\' )", "N�o foi possivel alterar tabela", 3);
			tentarComando(adminCon, "UPDATE user_info SET " + (resultado == 0 ? "abandoned = abandoned + 1" : 
				resultado == 1 ? "wins = wins + 1" :
					resultado == 2 ? "loses = loses + 1" : "draws = draws + 1") +
					" WHERE user_info.userId = " + idUsuario, "N�o foi possivel atualizar dados do usu�rio", 3);
			adminCon.close();
		}
	}
	
	public Vector<HistoricoJogo> retornarHistoricoPartidas() throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		ResultSet rs = tentarQuery(adminCon, "SELECT * FROM game_history", "N�o foi possivel retornar o hist�rico de partidas", 3);
		if(rs == null)
			return null;
		
		Vector<HistoricoJogo> data = new Vector<HistoricoJogo>();
		while(rs.next())
		{
			data.add(new HistoricoJogo(rs.getInt("gameId"), rs.getInt("userId"), rs.getString("game")));
		}
		
		adminCon.close();
		
		return data;
	}
	
	public void apagarHistorico(int userID) throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		tentarComando(adminCon, "UPDATE user_info SET " + "abandoned = 0, wins = 0, loses = 0, draws = 0" +
				" WHERE user_info.userId = " + userID, "N�o foi possivel atualizar dados do usu�rio", 3);
		adminCon.close();
	}
	
	private static Connection createAdminConnection() throws SQLException
	{
		return DriverManager.getConnection(hostName, adminUser, psswd);
	}
	
	public void restoreConnection(int userId) throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		tentarComando(adminCon, "CREATE OR REPLACE VIEW user_view AS "
				+ "SELECT * FROM user_info WHERE user_info.userId = " + userId + " GROUP BY user_info.userId", 
				"N�o foi possivel recriar view!", 3);
		adminCon.close();
	}
	
	public Connection createUserConnection(String username, String psswd) throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		Connection userConn = DriverManager.getConnection(hostName, username, psswd);
		tentarComando(adminCon, "CREATE OR REPLACE VIEW user_view AS "
				+ "SELECT * FROM user_info WHERE user_info.user = \"" + username + "\" GROUP BY user_info.userId", 
				"N�o foi possivel recriar view!", 3);
		adminCon.close();
		return userConn;
	}
	
	public static void endUserConnection(Connection userCon) throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		tentarComando(adminCon, "CREATE OR REPLACE VIEW user_view AS "
				+ "SELECT * FROM user_info WHERE user_info.userId = 0 GROUP BY user_info.userId",  
				"N�o foi possivel recriar view!", 3);
		adminCon.close();
	}
	
	public boolean retornarInfoUsuario() throws SQLException
	{
		Connection adminCon = createAdminConnection();
		tentarComando(adminCon, "USE " + banco, "N�o foi possivel usar banco de dados!", 3);
		ResultSet rs = tentarQuery(adminCon, "SELECT * FROM user_view", "N�o foi possivel retornar o nome do usu�rio", 3);
		if(rs == null)
			return false;
		rs.next();
		idUsuario = rs.getInt("userId");
		usuario = rs.getString("user");
		partidas = rs.getInt("games");
		partidasGanhas = rs.getInt("wins");
		partidasPerdidas = rs.getInt("loses");
		partidasEmpatadas = rs.getInt("draws");
		partidasAbandonadas = rs.getInt("abandoned");
		adminCon.close();
		
		return true;
	}
	
	public int retornarIdUsuario()
	{
		return idUsuario;
	}
	
	public String retornarNomeUsuario()
	{
		return usuario;
	}
	
	public int retornarPartidas()
	{
		return partidas;
	}
	
	public int retornarPartidasGanhas()
	{
		return partidasGanhas;
	}
	
	public int retornarPartidasPerdidas()
	{
		return partidasPerdidas;
	}
	
	public int retornarPartidasEmpatadas()
	{
		return partidasEmpatadas;
	}
	
	public int retornarPartidasAbandonadas()
	{
		return partidasAbandonadas;
	}
	
	@SuppressWarnings("unused")
	private static void tentarComando(Connection conn, String comando, String mensagemErro) throws SQLException
	{
		while(true)
		{
			try {
				PreparedStatement ps = conn.prepareStatement(comando);
				ps.execute();
				break;
			} catch(SQLException e)
			{
				int result = JOptionPane.showConfirmDialog(null, mensagemErro +
			"\nDeseja tentar novamente?" + "\nerror: " + e.getMessage(),
			"erro", JOptionPane.YES_NO_OPTION);
				if(result == JOptionPane.NO_OPTION)
				{
					conn.close();
					throw e;
				}
			}
		}
	}
	
	@SuppressWarnings("unused")
	private static void tentarComando(Connection conn, String comando, String mensagemErro, int maximoTentativas) throws SQLException
	{
		for(int i = 0; true; ++i)
		{
			try {
				PreparedStatement ps = conn.prepareStatement(comando);
				ps.execute();
				break;
			} catch(SQLException e)
			{
				if(i >= maximoTentativas)
				{
					conn.close();
					throw e;
				}
				int result = JOptionPane.showConfirmDialog(null, mensagemErro +
						"\nDeseja tentar novamente?" + "\nerror: " + e.getMessage(),
						"erro", JOptionPane.YES_NO_OPTION);
				if(result == JOptionPane.NO_OPTION)
				{
					conn.close();
					throw e;
				}
			}
		}
	}
	
	@SuppressWarnings("unused")
	private static ResultSet tentarQuery(Connection conn, String comando, String mensagemErro) throws SQLException
	{
		while(true)
		{
			try {
				PreparedStatement ps = conn.prepareStatement(comando);
				return ps.executeQuery();
			} catch(SQLException e)
			{
				int result = JOptionPane.showConfirmDialog(null, mensagemErro +
			"\nDeseja tentar novamente?" + "\nerror: " + e.getMessage(),
			"erro", JOptionPane.YES_NO_OPTION);
				if(result == JOptionPane.NO_OPTION)
				{
					conn.close();
					throw e;
				}
			}
		}
	}
	
	@SuppressWarnings("unused")
	private static ResultSet tentarQuery(Connection conn, String comando, String mensagemErro, int maximoTentativas) throws SQLException
	{
		for(int i = 0; true; ++i)
		{
			try {
				PreparedStatement ps = conn.prepareStatement(comando);
				return ps.executeQuery();
			} catch(SQLException e)
			{
				if(i >= maximoTentativas)
				{
					conn.close();
					throw e;
				}
				int result = JOptionPane.showConfirmDialog(null, mensagemErro +
						"\nDeseja tentar novamente?" + "\nerror: " + e.getMessage(),
						"erro", JOptionPane.YES_NO_OPTION);
				if(result == JOptionPane.NO_OPTION)
				{
					conn.close();
					throw e;
				}
			}
		}
	}
}