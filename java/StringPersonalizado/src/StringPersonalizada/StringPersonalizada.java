package StringPersonalizada;

public class StringPersonalizada {
	
	//variáveis de instancia
	
	/**
	 * string atual.
	 */
	String minhaString;
	
	/**
	 * indica se a string atual minhaString já foi definido
	 */
	boolean estaDefinido;
	
	//constructor
	
	/**
	 * Inicializa minhaString to null.
	 * Inicializa estaDefinido para falso.
	 */
	public StringPersonalizada() {
		this.minhaString = null;
		this.estaDefinido = false;
	}

	// métodos 
	
    /**
     * Retorna a String atual.
     * Se a string atual é null, ou ainda não tiver um valor definido pelo método defineString, deve retornar null.
     * @return string atual
     */
	public String pegaString() {	
		return this.minhaString;
	}

	/**
	 * Define o valor da string atual e define this.estaDefinido para true.
	 * Se a string dada for null (se estiver vazia), define this.estaDefinido para falso.
     * @param string para ser definida
     */
	public void defineString(String string) {
		if(string != null)
			this.estaDefinido = true;
		else
			this.estaDefinido = false;
		
		this.minhaString = string;
	}

    /**
     * Retorna uma nova versão da string onde os caracteres alfabéticos dados no argumento, são removidos.
     * Todos os caracteres NÃO ALFABÉTICOS não são afetados
     * Se a string atual é null, vazia, ou ainda não tem valor definido, esse método deve retornar uma string vazia.
     * Se lembre: Esse método constrói e retorna uma nova string, e não modifica diretamente o campo minhaString.
     * @param arg	a string contendo os caracteres a serem removidos 
     * @return 		nova string no qual os caracteres especificados em arg são removidos
     */
	public String remove(String arg) {
		String novaString = "";
		
		if(this.minhaString == null)
			return novaString;
		
		int token = 0, proxToken;
		while(token < this.minhaString.length())
		{
			proxToken = Integer.MAX_VALUE;
			for(int i = 0; i < arg.length(); ++i)
			{
				if(Character.isAlphabetic(arg.charAt(i)))
				{
					proxToken = Math.min(this.minhaString.indexOf(arg.charAt(i), token) & Integer.MAX_VALUE, proxToken);
					proxToken = Math.min(proxToken, this.minhaString.length());
				}
			}
			novaString += this.minhaString.substring(token, proxToken);
			token = proxToken+1;
		}
		
	  	return novaString;
	}
	
	/**
	 * Retorna uma nova string onde a capitalização é revertida (i.e. maiúscula para mimúscula, e mimúscula para maiúscula) de acordo 
	 * com os caracteres específicados no argumento (arg).
	 * TODOS os caracteres nao alfabéticos são inafetados
	 * Se a string atual é null, vazia, ou seja, se ainda não tem valor definido, esse método deve retornar uma string vazia.
	 *
	 * Examplo:
	 * - Para a string "abc, XYZ; 123.", chamar reverte("bcdxyz@3210.") retornaria "aBC, xyz; 123."
	 * 
	 * Se lembre: Esse método constrói e retorna uma nova string, e não modifica diretamente o campo minhaString.
	 * 
	 * @param arg a string contendo os caracteres a serem modificados
	 * @return uma nova string no qual capitalização é revertida
	 */
	public String reverte(String arg) {
		String novaString = "";
		
		if(this.minhaString == null)
			return novaString;
		
		arg = arg.toLowerCase();
		arg += arg.toUpperCase();
		
		int token = 0, proxToken;
		while(token < this.minhaString.length())
		{
			proxToken = Integer.MAX_VALUE;
			for(int i = 0; i < arg.length(); ++i)
			{
				if(Character.isAlphabetic(arg.charAt(i)))
				{
					proxToken = Math.min(this.minhaString.indexOf(arg.charAt(i), token) & Integer.MAX_VALUE, proxToken);
					proxToken = Math.min(proxToken, this.minhaString.length());
				}
			}
			novaString += this.minhaString.substring(token, proxToken);
			
			if(proxToken != this.minhaString.length())
			{
				if(Character.isUpperCase(this.minhaString.charAt(proxToken)))
					novaString += Character.toLowerCase(this.minhaString.charAt(proxToken));
				else if(Character.isLowerCase(this.minhaString.charAt(proxToken)))
					novaString += Character.toUpperCase(this.minhaString.charAt(proxToken));
			}

			token = proxToken+1;
		}
		
		return novaString;
	}

    /**
     * Retorna uma nova string em que todas as letras ou >= ou <= que o char n, são removidos.  
     * 
     * A letra fornecida pode ser a-z ou A-Z.
     * As letras a serem removidas não diferenciam maiúsculas de minúsculas.
     *
     * Se a string atual é null, vazia, ou seja, se ainda não tem valor definido, esse método deve retornar uma string vazia.
     * 
     *
     * Se n não for uma letra (e a string atual não for nula), o método deve retornar uma string vazia.
     *
     * Examplo:
     * - Para a string "Hello 90, bye 2", chamar filtraLetras('h', false) retornaria "llo 90, y 2"
     * 
     * Se lembre: Esse método constrói e retorna uma nova string, e não modifica diretamente o campo minhaString.
     *
     * @param n 		char para comparar 
     * @param more 		indica se letras >= ou <= serão removidas
     * @return nova string com as letras removidas
     */
	public String filtraLetras(char n, boolean mais) {
		String novaString = "";
		
		if(this.minhaString == null)
			return novaString;
		
		for(int i = 0; i < this.minhaString.length(); ++i)
		{
			if(Character.isAlphabetic(this.minhaString.charAt(i)))
			{
				if(mais == true && Character.toLowerCase(this.minhaString.charAt(i)) < Character.toLowerCase(n))
				{
					novaString += this.minhaString.charAt(i);
				}
				else if(mais == false && Character.toLowerCase(this.minhaString.charAt(i)) > Character.toLowerCase(n))
				{
					novaString += this.minhaString.charAt(i);
				}
			}
			else
			{
				novaString += this.minhaString.charAt(i);
			}
		}
	  	return novaString;
	}
}