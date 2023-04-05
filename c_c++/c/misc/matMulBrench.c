#include<windows.h>

#include<stdlib.h>
#include<stdio.h>
#include<xmmintrin.h>
#include<emmintrin.h>
#include<string.h>

#include"cacheLine.h"

#define MATRIX_SIZE 1000
#define TEST_COUNT 4
//#define LINE_SIZE sizeof(SIMDmatTy)/sizeof(matTy);

/*#define SIMD_MUL(A, B) _mm_mul_epu32(A, B)
#define SIMD_SET(A) _mm_set_epi32(A, A+sizeof(matTy), A+2*sizeof(matTy), A+3*sizeof(matTy))
#define SIMD_SET_EACH(A, B, C, D) _mm_set_epi32(A, B, C, D)
#define SIMD_ADD(A, B) _mm_add_epi32(A, B)*/

#define SIMD_MUL(A, B) _mm_mul_pd(A, B)
#define SIMD_ADD(A, B) _mm_add_pd(A, B)
#define SIMD_LOAD(A) _mm_load_pd(A)
#define SIMD_LOAD_REPEAT(A) _mm_load_pd1(A)
#define SIMD_STORE(A, B) _mm_store_pd(A, B)
#define SIMD_SET(A) _mm_set_pd(A, A+sizeof(matTy), A+2*sizeof(matTy), A+3*sizeof(matTy))
#define SIMD_SET_EACH(A, B) _mm_set_pd(A, B)

#define REF test1
#define REF_NS test1Ns

typedef double __attribute__((aligned(64))) matTy;
typedef __m128d SIMDmatTy;

int main()
{
	/*matTy* mat1 = calloc(MATRIX_SIZE*MATRIX_SIZE, sizeof(matTy));
	matTy* mat2 = calloc(MATRIX_SIZE*MATRIX_SIZE, sizeof(matTy));
	matTy* temp = calloc(MATRIX_SIZE*MATRIX_SIZE, sizeof(matTy));
	matTy* res = calloc(MATRIX_SIZE*MATRIX_SIZE, sizeof(matTy));*/

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	size_t cacheLine = getCacheLine()/sizeof(matTy);
	cacheLine = cacheLine > 0 ? cacheLine : 1;

	matTy** mat1 = calloc(MATRIX_SIZE, sizeof(matTy*));
	for(size_t i = 0; i < MATRIX_SIZE; ++i)
		mat1[i] = calloc(MATRIX_SIZE + (~(MATRIX_SIZE-1) & (sizeof(SIMDmatTy)/sizeof(matTy)-1)), sizeof(matTy));

	matTy** mat2 = calloc(MATRIX_SIZE, sizeof(matTy*));
	for(size_t i = 0; i < MATRIX_SIZE; ++i)
		mat2[i] = calloc(MATRIX_SIZE + (~(MATRIX_SIZE-1) & (sizeof(SIMDmatTy)/sizeof(matTy)-1)), sizeof(matTy));

	matTy** temp = calloc(MATRIX_SIZE, sizeof(matTy*));
	for(size_t i = 0; i < MATRIX_SIZE; ++i)
		temp[i] = calloc(MATRIX_SIZE + (~(MATRIX_SIZE-1) & (sizeof(SIMDmatTy)/sizeof(matTy)-1)), sizeof(matTy));

	matTy** res = calloc(MATRIX_SIZE, sizeof(matTy*));
	for(size_t i = 0; i < MATRIX_SIZE; ++i)
		res[i] = calloc(MATRIX_SIZE + (~(MATRIX_SIZE-1) & (sizeof(SIMDmatTy)/sizeof(matTy)-1)), sizeof(matTy));

	//data init
	for(size_t i = 0; i < MATRIX_SIZE; ++i)
		for(size_t j = 0; j < MATRIX_SIZE; ++j)
			//res[j+i*sizeof(matTy)] = 0;
			res[i][j] = 0;

	for(size_t i = 0; i < MATRIX_SIZE; ++i)
		for(size_t j = 0; j < MATRIX_SIZE; ++j)
			//mat1[j+i*sizeof(matTy)] = rand();
			mat1[i][j] = rand();

	for(size_t i = 0; i < MATRIX_SIZE; ++i)
		for(size_t j = 0; j < MATRIX_SIZE; ++j)
			//mat2[j+i*sizeof(matTy)] = rand();
			mat2[i][j] = rand();

	printf("cache line size = %d\n", cacheLine);

	//test 1
	double test1 = 0.0;
	for(size_t i = 0; i < TEST_COUNT; ++i)
	{
	    LARGE_INTEGER startCounter, endCounter;
	    QueryPerformanceCounter(&startCounter);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				for(size_t k = 0; k < MATRIX_SIZE; ++k)
					//res[j+i*sizeof(matTy)] += mat1[k+i*sizeof(matTy)] * mat2[j+k*sizeof(matTy)];
					res[i][j] += mat1[i][k] * mat2[k][j];

		QueryPerformanceCounter(&endCounter);
		test1 += (endCounter.QuadPart-startCounter.QuadPart);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//res[j+i*sizeof(matTy)] = 0;
				res[i][j] = 0;
	}
	test1 /= TEST_COUNT*freq.QuadPart;
	printf("test1: %f\n", test1);

	//test 2
	double test2 = 0.0;
	for(size_t i = 0; i < TEST_COUNT; ++i)
	{
	    LARGE_INTEGER startCounter, endCounter;
	    QueryPerformanceCounter(&startCounter);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//temp[j+i*sizeof(matTy)] = mat2[i+j*sizeof(matTy)];
				temp[i][j] = mat2[j][i];

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				for(size_t k = 0; k < MATRIX_SIZE; ++k)
					//res[j+i*sizeof(matTy)] += mat1[k+i*sizeof(matTy)] * temp[k+j*sizeof(matTy)];
					res[i][j] += mat1[i][k] * temp[j][k];

		QueryPerformanceCounter(&endCounter);
		test2 += (endCounter.QuadPart-startCounter.QuadPart);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//res[j+i*sizeof(matTy)] = 0;
				res[i][j] = 0;
	}
	test2 /= TEST_COUNT*freq.QuadPart;
	printf("test2: %f\n", test2);

	//test 3
	double test3 = 0.0;
	for(size_t i = 0; i < TEST_COUNT; ++i)
	{
	    LARGE_INTEGER startCounter, endCounter;
	    QueryPerformanceCounter(&startCounter);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t k = 0; k < MATRIX_SIZE; ++k)
				for(size_t j = 0; j < MATRIX_SIZE; ++j)
					//res[j+i*sizeof(matTy)] += mat1[k+i*sizeof(matTy)] * temp[k+j*sizeof(matTy)];
					res[i][j] += mat1[i][k] * mat2[k][j];

		QueryPerformanceCounter(&endCounter);
		test3 += (endCounter.QuadPart-startCounter.QuadPart);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//resa[j+i*sizeof(matTy)] = 0;
				res[i][j] = 0;
	}
	test3 /= TEST_COUNT*freq.QuadPart;
	printf("test3: %f\n", test3);

	//test 4
	double test4 = 0.0;
	for(size_t i = 0; i < TEST_COUNT; ++i)
	{
	    matTy* restrict rmat1;
	    matTy* restrict rmat2;
	    matTy* restrict rres;

        LARGE_INTEGER startCounter, endCounter;
	    QueryPerformanceCounter(&startCounter);

		for(size_t i = 0; i < MATRIX_SIZE; i += cacheLine)
			for(size_t k = 0; k < MATRIX_SIZE; k += cacheLine)
                for(size_t j = 0; j < MATRIX_SIZE; j += cacheLine)
                {
                    matTy* restrict rmat1 = &mat1[i][k];
                    matTy* restrict rres = &res[i][j];
                    for(size_t i2 = 0; i2 < cacheLine && i+i2 < MATRIX_SIZE; ++i2, rres = &res[i+i2][j], rmat1 = &mat1[i+i2][k])
                    {
                        //printf("working i\n");
                        matTy* restrict rmat2 = &mat2[k][j];
                        for(size_t k2 = 0; k2 < cacheLine && k+k2 < MATRIX_SIZE; ++k2, rmat2 = &mat2[k+k2][j])
                        {
                            //printf("working k\n");
                            for(size_t j2 = 0; j2 < cacheLine && j+j2 < MATRIX_SIZE; ++j2)
                            {
                                //printf("working j\n");
                                rres[j2] += rmat1[k2] * rmat2[j2];
                            }
                        }
                    }
                }

		QueryPerformanceCounter(&endCounter);
		test4 += (endCounter.QuadPart-startCounter.QuadPart);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//resa[j+i*sizeof(matTy)] = 0;
				res[i][j] = 0;
	}
	test4 /= TEST_COUNT*freq.QuadPart;
	printf("test4: %f\n", test4);

	//test 5
	double test5 = 0.0;
	for(size_t i = 0; i < TEST_COUNT; ++i)
	{
	    LARGE_INTEGER startCounter, endCounter;
	    QueryPerformanceCounter(&startCounter);

		matTy* restrict rmat1;
	    matTy* restrict rmat2;
	    matTy* restrict rres;

		rres = &res[0][0];
		rmat1 = &mat1[0][0];
		for(size_t i = 0; i < MATRIX_SIZE; ++i, rres = &res[i][0], rmat1 = &mat1[i][0])
        {
            rmat2 = &mat2[0][0];
			for(size_t k = 0; k < MATRIX_SIZE; ++k, rmat2 = &mat2[k][0])
            {
				for(size_t j = 0; j < MATRIX_SIZE; ++j)
					res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }

		QueryPerformanceCounter(&endCounter);
		test5 += (endCounter.QuadPart-startCounter.QuadPart);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//resa[j+i*sizeof(matTy)] = 0;
				res[i][j] = 0;
	}
	test5 /= TEST_COUNT*freq.QuadPart;
	printf("test5: %f\n", test5);

	//test 6
	double test6 = 0.0;
	for(size_t i = 0; i < TEST_COUNT; ++i)
	{
	    LARGE_INTEGER startCounter, endCounter;
	    QueryPerformanceCounter(&startCounter);

		matTy* restrict rmat1;
	    matTy* restrict rmat2;
	    matTy* restrict rres;

		rres = &res[0][0];
		rmat1 = &mat1[0][0];
		for(size_t i = 0; i < MATRIX_SIZE; ++i, rres = &res[i][0], rmat1 = &mat1[i][0])
        {
            rmat2 = &mat2[0][0];
			for(size_t k = 0; k < MATRIX_SIZE; ++k, rmat2 = &mat2[k][0])
            {
                    SIMDmatTy smat1 = SIMD_LOAD_REPEAT(&rmat1[k]);
                    for(size_t j = 0; j < MATRIX_SIZE; j += sizeof(SIMDmatTy)/sizeof(matTy))
                    {
                        SIMDmatTy smat2 = SIMD_LOAD(&rmat2[j]);
                        SIMDmatTy sres = SIMD_LOAD(&rres[j]);
                        SIMD_STORE(&rres[j], SIMD_ADD(sres, SIMD_MUL(smat1, smat2)));
                    }
            }
        }

		QueryPerformanceCounter(&endCounter);
		test6 += (endCounter.QuadPart-startCounter.QuadPart);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//resa[j+i*sizeof(matTy)] = 0;
				res[i][j] = 0;
	}
	test6 /= TEST_COUNT*freq.QuadPart;
	printf("test6: %f\n", test6);

    //test 7
	double test7 = 0.0;
	for(size_t i = 0; i < TEST_COUNT; ++i)
	{
	    LARGE_INTEGER startCounter, endCounter;
	    QueryPerformanceCounter(&startCounter);

		for(size_t i = 0; i < MATRIX_SIZE; i += cacheLine)
			for(size_t k = 0; k < MATRIX_SIZE; k += cacheLine)
                for(size_t j = 0; j < MATRIX_SIZE; j += cacheLine)
                {
                    matTy* restrict rmat1 = &mat1[i][k];
                    matTy* restrict rres = &res[i][j];
                    for(size_t i2 = 0; i2 < cacheLine && i+i2 < MATRIX_SIZE; ++i2, rres = &res[i+i2][j], rmat1 = &mat1[i+i2][k])
                    {
                        matTy* restrict rmat2 = &mat2[k][j];
                        for(size_t k2 = 0; k2 < cacheLine && k+k2 < MATRIX_SIZE; ++k2, rmat2 = &mat2[k+k2][j])
                        {
                            SIMDmatTy smat1 = SIMD_LOAD_REPEAT(&rmat1[k2]);
                            for(size_t j2 = 0; j2 < cacheLine && j+j2 < MATRIX_SIZE; j2 += sizeof(SIMDmatTy)/sizeof(matTy))
                            {
                                SIMDmatTy smat2 = SIMD_LOAD(&rmat2[j2]);
                                SIMDmatTy sres = SIMD_LOAD(&rres[j2]);
                                SIMD_STORE(&rres[j2], SIMD_ADD(sres, SIMD_MUL(smat1, smat2)));
                            }
                        }
                    }
                }

		QueryPerformanceCounter(&endCounter);
		test7 += (endCounter.QuadPart-startCounter.QuadPart);

		for(size_t i = 0; i < MATRIX_SIZE; ++i)
			for(size_t j = 0; j < MATRIX_SIZE; ++j)
				//resa[j+i*sizeof(matTy)] = 0;
				res[i][j] = 0;
	}
	test7 /= TEST_COUNT*freq.QuadPart;
	printf("test7: %f\n", test7);

	//results
	printf("\n");
	printf("test 1: %f\ntest 2: %f\ntest 3: %f\ntest 4: %f\ntest 5: %f\ntest 6: %f\ntest 7: %f\n\n", test1/REF, test2/REF, test3/REF, test4/REF, test5/REF, test6/REF, test7/REF );

    double sort[] = {test1, test2, test3, test4, test5, test6, test7};
    int indexes[] = {1, 2, 3, 4, 5, 6, 7};
    for(size_t i = 0; i < sizeof(sort)/sizeof(double); ++i)
        for(size_t j = i; j < sizeof(sort)/sizeof(double); ++j)
        {
            if(sort[i] > sort[j])
            {
                double temp = sort[i];
                sort[i] = sort[j];
                sort[j] = temp;
                int temp2 = indexes[i];
                indexes[i] = indexes[j];
                indexes[j] = temp2;
            }
        }
    printf("test %d\ntest %d\ntest %d\ntest %d\ntest %d\ntest %d\ntest %d\n", indexes[0], indexes[1], indexes[2], indexes[3], indexes[4], indexes[5], indexes[6]);

	getchar();

	return 0;
}
