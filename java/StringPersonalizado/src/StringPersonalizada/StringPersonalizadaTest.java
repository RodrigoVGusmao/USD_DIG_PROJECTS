package StringPersonalizada;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class StringPersonalizadaTest {

	// declara uma string personazliada para teste
	StringPersonalizada minhaStringPersonalizada;

	@BeforeEach
	public void setUp() throws Exception {
		//inicializa uma string personalizada
		this.minhaStringPersonalizada = new StringPersonalizada();
	}

	@Test
	void testGetString() {
		
		//string deve ser null para começar, antes de inicia-la
		assertNull(this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("hello");
		assertEquals("hello", this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("");
		assertEquals("", this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("123456789");
		assertEquals("123456789", this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("@#$%¨&¹²³");
		assertEquals("@#$%¨&¹²³", this.minhaStringPersonalizada.pegaString());
	}
	
	@Test
	void testDefineString() {
		
		//string deve ser null para começar, antes de inicia-la
		assertNull(this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("Good-bye!");
		assertEquals("Good-bye!", this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("");
		assertEquals("", this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("123456789");
		assertEquals("123456789", this.minhaStringPersonalizada.pegaString());
		
		this.minhaStringPersonalizada.defineString("@#$%¨&¹²³");
		assertEquals("@#$%¨&¹²³", this.minhaStringPersonalizada.pegaString());
	}
	
	@Test
	void testRemove() {
		assertEquals("", this.minhaStringPersonalizada.remove(""));
		
		this.minhaStringPersonalizada.defineString(null);
		assertEquals("", this.minhaStringPersonalizada.remove(""));
		
		this.minhaStringPersonalizada.defineString("Estudo no Ceub em 2021");
		assertEquals("Etud n Ceub em 2021", this.minhaStringPersonalizada.remove("so2"));
		
		this.minhaStringPersonalizada.defineString("BEN-10");
		assertEquals("E-10", this.minhaStringPersonalizada.remove("BeN-10"));
		
		this.minhaStringPersonalizada.defineString("Aa@Bb");
		assertEquals("A@B", this.minhaStringPersonalizada.remove("@ba"));
		
		this.minhaStringPersonalizada.defineString("1a2b3c4d");
		assertEquals("123c4", this.minhaStringPersonalizada.remove("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@dba"));
	}

	@Test
	void testReverse() {
		assertEquals("", this.minhaStringPersonalizada.reverte(""));
		
		this.minhaStringPersonalizada.defineString(null);
		assertEquals("", this.minhaStringPersonalizada.reverte(""));
		
		this.minhaStringPersonalizada.defineString("abc, XYZ; 123.");
		assertEquals("aBC, xyz; 123.", this.minhaStringPersonalizada.reverte("bcdxyz@3210."));
		
		this.minhaStringPersonalizada.defineString("abcdefg@");
		assertEquals("AbCdEfG@", this.minhaStringPersonalizada.reverte("aceg@"));
		
		assertEquals("AbCdEfG@", this.minhaStringPersonalizada.reverte("@aceg"));
		
		this.minhaStringPersonalizada.defineString("@gfedcba");
		assertEquals("@GfEdCbA", this.minhaStringPersonalizada.reverte("aceg@"));
	}

	@Test
	void testfiltraLetras() {
		assertEquals("", this.minhaStringPersonalizada.filtraLetras('E', false));
		
		this.minhaStringPersonalizada.defineString(null);
		assertEquals("", this.minhaStringPersonalizada.filtraLetras('E', false));
		
		this.minhaStringPersonalizada.defineString("123456789abcdefg");
		assertEquals("123456789efg", this.minhaStringPersonalizada.filtraLetras('d', false));
		
		/*assertEquals("123456789abc", this.minhaStringPersonalizada.filtraLetras('d', true));
		
		assertEquals("123456789abc", this.minhaStringPersonalizada.filtraLetras('D', true));*/
	}

}