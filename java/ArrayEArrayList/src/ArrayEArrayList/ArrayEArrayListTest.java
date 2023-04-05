package ArrayEArrayList;

import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class ArrayEArrayListTest {

	// declara minhaArray que e o objeto de testes
	ArrayEArrayList minhaArray;
	
	@BeforeEach
	void setUp() throws Exception {
		//inicializa para testes
		this.minhaArray = new ArrayEArrayList();
	}
	
	/**
	 * testa o metodo quantos em array e arrayList.
	 */
	@Test
	void quantosTest() {
		// elemento na array
		int[] array = {1, 3, 5, 7, 9, 1, 2, 3, 4, 5};
		assertEquals(2, this.minhaArray.quantos(array, 1));
		
		assertEquals(0, this.minhaArray.quantos(array, 6));
		
		assertEquals(2, this.minhaArray.quantos(array, 5));
		
		assertEquals(1, this.minhaArray.quantos(array, 7));
	}
	
	/**
	 * Test encontraMax.
	 */
	@Test
	void testFindMax() {
		
		int[] array = {1, 3, 5, 7, 9, 1, 2, 3, 4, 5};
		assertEquals(9, this.minhaArray.encontraMax(array));
		
		array[4] = 'a';
		assertEquals('a', this.minhaArray.encontraMax(array));
		
		array[4] = 0;
		assertEquals(7, this.minhaArray.encontraMax(array));
		
		array[3] = -1;
		assertEquals(5, this.minhaArray.encontraMax(array));
		
	}

	/**
	 * Test quantosMax
	 */
	@Test
	void testQuantosMax() {
		 
		int[] array = {1, 3, 5, 7, 9, 1, 2, 3, 4, 5};
		ArrayList<Integer> testArrayList = new ArrayList<Integer>();
		testArrayList.add(9);
		assertEquals(testArrayList, this.minhaArray.quantosMax(array));
		
		array[4] = 0;
		testArrayList.set(0, 7);
		assertEquals(testArrayList, this.minhaArray.quantosMax(array));
		
		array[0] = 7;
		testArrayList.add(7);
		assertEquals(testArrayList, this.minhaArray.quantosMax(array));
		
		array[1] = 7;
		testArrayList.add(7);
		assertEquals(testArrayList, this.minhaArray.quantosMax(array));
	}

	/**
	 * Test trocaZero
	 */
	@Test
	void testTrocaZero() {
		// testa array contendo 0's
		int[] array = {0, 1, 0, 2, 0, 3, 0, 5};
		int[] testArray = {1, 2, 3, 5, 0, 0, 0, 0};
		this.minhaArray.trocaZero(array);
		assertArrayEquals(testArray, array);
		
		
		int[] array2 = {0, 0, 0, 0, 0, 0, 0, 0};
		this.minhaArray.trocaZero(array2);
		assertArrayEquals(array2, array2);
		
		int[] array3 = {0, 1, 0, 2, 0, 3, 0, 5, 0};
		int[] testArray3 = {1, 2, 3, 5, 0, 0, 0, 0, 0};
		this.minhaArray.trocaZero(array3);
		assertArrayEquals(testArray3, array3);
		
		int[] array4 = {1, 2, 3};
		int[] testArray4 = {1, 2, 3};
		this.minhaArray.trocaZero(array4);
		assertArrayEquals(testArray4, array4);
		
	}
}