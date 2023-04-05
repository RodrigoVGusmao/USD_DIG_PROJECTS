package ArrayEArrayList;

import java.util.ArrayList;

/**
 * Classe com metodos para trabalhar com e manipular Arrays e ArrayLists
 */
public class ArrayEArrayList {

	/**
	 * Conta o numero de ocorrencias de um valor em uma array
	 * @param array para procurar
	 * @param elemento a se procurar
	 * @return numero de vezes que o elemento se repete na array
	 */
	public int quantos(int[] array, int elemento) {
		int ocorrencias = 0;
		
		for(int i = 0; i < array.length; ++i)
		{
			if(array[i] == elemento)
				++ocorrencias;
				
		}
		return ocorrencias;
	}
	
	/**
	 * Encontra o maior valor dentro da array.
	 * Se array estiver vazia, retorna -1.
	 * 
	 * @param array eh  procurar
	 * @return maior numero na array
	 */
	public int encontraMax(int[] array) {
		int maiorValor = -1;
		for(int i = 0; i < array.length; ++i)
		{
			maiorValor = Math.max(maiorValor, array[i]);
		}
		return maiorValor;
	}
	
	/**
	 * Pega todas as ocorrencias do maior numero na array
	 * E retorna uma ArrayList que contem cada ocorrencia desse numero 
	 * Usa o metodo encontraMax(int[] array) 
	 * Se a array esta vazia, retorna Null.
	 * 
	 * Examplo(s):
     * - Para a array: int[] array = {1, 3, 5, 7, 9, 1, 2, 3, 4, 5};
     * - Chamar maxArray(array) retornaria uma ArrayList contendo 9
     * 
     * - Para a array: int[] array = {2, 4, 8, 12, 12, 4};
     * - Chamar maxArray(array) retornaria uma ArrayList contendo 12 e 12
     * 
     * - Para a array: int[] array = new int[0];
     * - Chamar maxArray(array) retornaria null
     * 
	 * @param array para procurar
	 * @return ArrayList contendo cada instancia do maior numero
	 */
	public ArrayList<Integer> quantosMax(int[] array) {
		if(array.length == 0)
			return null;
		
		int maxValue = this.encontraMax(array);
		ArrayList<Integer> retorno = new ArrayList<Integer>(this.quantos(array, maxValue));
		for(int i = 0; i < this.quantos(array, maxValue); ++i)
		{
			retorno.add(maxValue);
		}
		
		return retorno;
	}
	
	/**
	 * Poe todos os 0's do array no fim.
	 * Faz esse update no proprio array.
	 * mantem ordem dos nao-zeros.
	 * 
	 * @param array a  se procurar por zeros.
	 */
	public void trocaZero(int[] array) {
		int zeros = this.quantos(array, 0);
		
		for(int i = 0; i < array.length; ++i)
		{
			if(array[i] == 0 && i < array.length-zeros)
			{
				for(int j = i--; j < array.length-1;)
				{
					array[j] = array[++j];
				}
			}
			if(i >= array.length-zeros)
			{
				array[i] = 0;
			}
		}
	}
}