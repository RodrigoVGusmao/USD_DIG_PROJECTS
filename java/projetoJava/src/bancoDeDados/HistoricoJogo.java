package bancoDeDados;

public class HistoricoJogo {
	public int gameId;
	public int userId;
	public String JSON;
	
	public HistoricoJogo(int gameId, int userId, String JSON) 
	{
		this.gameId = gameId;
		this.userId = userId;
		this.JSON = JSON;
	}
	
	@Override
	public String toString()
	{
		return gameId + " " + userId + " " + JSON;
	}
}