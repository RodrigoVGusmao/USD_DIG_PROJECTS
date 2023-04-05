package prova2;

public class Calculo {
	
	public static double potencia(double base, int exp)
	{
		double ret = 1;
		if(exp < 0)
		{
			exp *= -1;
			base = 1/base;
		}
		
		for(int i = 0; i < exp; ++i)
		{
			ret *= base;
		}
		
		return ret;
	}
}
