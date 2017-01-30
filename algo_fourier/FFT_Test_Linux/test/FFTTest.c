/*
 *
 *
 * File: FFTTest.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FFTTest.h"

#define VERBOSECOMP 0
#define TIMING 1
#define FFT_forward 1
#define FFT_inverse 0

//#define MAXTEST 4
//static FFTFunction FFTFunctiontest[MAXTEST];
//static int NumTestFunctions;

void RunFFT(complex *, int);
void RunFFTR(complex *, int);
void RunIFFT(complex *, int);
void RunIFFTR(complex *, int);
#define MAXTESTS 8
static FFTTest *test;


#define NEWTEST(Func, Di) { \
  test->FFTpoint = Func; \
  test->FuncName = #Func;\
  test->Direction = Di; \
}

static void InitialiseTests(void) {
  test = (FFTTest *)malloc(sizeof(FFTTest));
  NEWTEST(RunFFT, FFT_forward);
  //NEWTEST(RunFFTR, FFT_forward);
  //NEWTEST(RunIFFT, FFT_inverse);
  //NEWTEST(RunIFFTR, FFT_inverse);
}

static int compare_output(const FFTData *FFTDatatest, complex *Actin, int direction)
{
	int i, err, N;

	double d;
	double max = 0, accum = 0;
	
	complex *Refout;
	N = FFTDatatest->N;
  	Refout = (complex *)malloc(N*sizeof(complex));

        if(direction == FFT_forward)
	Refout = FFTDatatest->out;
	else
	Refout = FFTDatatest->in;

	printf("Ref:");
	for(i=0; i< 10 && i < N; i++)
	{
	printf("%.1f", Refout[i].real);

	if(Refout[i].imag>=0)
	printf("+%.1fi ", Refout[i].imag);
	else
	printf("%.1fi ", Refout[i].imag);
	}

	printf("\nAct:");
	for(i=0; i< 10 && i < N; i++)
	{
	printf("%.1f", Actin[i].real);

	if(Actin[i].imag>=0)
	printf("+%.1fi ", Actin[i].imag);
	else
	printf("%.1fi ", Actin[i].imag);
	}

	for(i = 0; i < N ; i++)
	{
		d = Actin[i].real - Refout[i].real;
		if(d <0) d = -d;
		if(d>max) max= d;
			
		accum +=d;
			
		d = Actin[i].imag - Refout[i].imag;
		if(d <0) d = -d;
		if(d>max) max= d;

		accum +=d;

	#if VERBOSECOMP
		
	printf("\nRef[%d] %.2f", i, Refout[i].real);

	if(Refout[i].imag>=0)
	printf("+%.2fi", Refout[i].imag);
	else
	printf("%.2fi", Refout[i].imag);

	printf("            \tAct[%d] %.2f", i,Actin[i].real);

	if(Actin[i].imag>=0)
	printf("+%.2fi", Actin[i].imag);
	else
	printf("%.2fi", Actin[i].imag);
	
	#endif

	}

	printf("\nMax error %.8f \n", max);
	printf("sum error %.8f \n", accum);	
	if(max > 10) return 1;
	else	
 		     return 0;
}//end of compare_output


static complex *LoadData(const FFTData *FFTDatatest, int direction)
{
	int N , i;
	complex *in;
	N = FFTDatatest->N;
  	in = (complex *)malloc(N*sizeof(complex));

	if(direction == FFT_forward)
	{	
	printf("\nTesting %d point %s FFT's\n", N, test->FuncName);
		for(i=0;i<N;i++)
		{
	            in[i].real = FFTDatatest->in[i].real;
		    in[i].imag = FFTDatatest->in[i].imag;
		}
        }
	else 
	{
	printf("\nTesting %d point %s inverse FFT's\n", N, test->FuncName);
		for(i=0;i<N;i++)
		{
	            in[i].real = FFTDatatest->out[i].real;
		    in[i].imag = FFTDatatest->out[i].imag;
		}
	}
	/*
	printf("*********** Donnes *****************\n");
	for(i=0;i<N;i++)
	{
		printf("%.8f\t\t",in[i].real);
		printf("%.8f\n",in[i].imag);
	}
	*/
	return in;
} 

static int TestFFT(const FFTData *FFTDatatest) 
{
  int N,i,failed = 0;
  complex *in;

    FILE *fp;

#if TIMING
  int start;
#endif
  	N = FFTDatatest->N;
	in = LoadData(FFTDatatest,test->Direction);

#if TIMING
  start = clock();
#endif

/*
fp = fopen("t000256.c", "wt");
fprintf(fp,"#include \"FFTTest.h\"\n");
fprintf(fp,"static const complex ffttest_256_in[256] = {\n");

	for(i = 0; i < 255; i++)
	{
	fprintf(fp,"\t{ %.0f,    \t%.0f },\n",in[i].real, in[i].imag);
	}
	fprintf(fp,"\t{ %.0f,    \t%.0f }\n};",in[255].real, in[255].imag);
*/
	test->FFTpoint(in, N);
/*
fprintf(fp,"\nstatic const complex ffttest_256_out[256] ={\n");
	for(i = 0; i < 255; i++)
	{
	fprintf(fp,"\t{ %.5f,    \t%.5f },\n",in[i].real, in[i].imag);
	}
	fprintf(fp,"\t{ %.5f,    \t%.5f }\n};",in[255].real, in[255].imag);

*/
close(fp);

#if TIMING
  start = clock()-start;
  printf("  Time taken = %d\n", start);
#endif


	printf("Performe :\n");
	failed = compare_output(FFTDatatest, in, test->Direction);

    return failed;
}

 
#define TestSize(n) { \
  extern const FFTData FFTData_##n; \
  failed += TestFFT(&FFTData_##n); \
}

main()
{
    int failed = 0;

    printf("Starting tests...\n");
    InitialiseTests();

 // TestSize(8);
  TestSize(16);
 // TestSize(32);
 // TestSize(64);
 // TestSize(128);
 // TestSize(256);
 // TestSize(512);
 // TestSize(1024);

    printf("\nFinished\n");
}

