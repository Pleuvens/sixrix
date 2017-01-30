/***********************************
// DIT radix-4  FFT  complex 
//
// 1. Maximium points are 1024.
//
// 2. The last stage is 2 DFT ( for 8, 32, 128, 512...)
// or all stages are 4 DFT ( for 4, 16, 64, 256, 1024 ...).
//
// 3. The functions special for FFT real .
//
// 24 juillet 2007
// purcharse*gmail.com
************************************/
#include "FFT.h"

static  complex multicomplex( complex b1, complex b2)  /* multiplication of complex */
{
 complex b3;
b3.real=b1.real*b2.real-b1.imag*b2.imag;
b3.imag=b1.real*b2.imag+b1.imag*b2.real;
return(b3);
}

static int mylog2(int N) 	/* Max(N) = 4098 */
{
  int k=0;
  
  if (N>>12) { k+=12; N>>=12; }
  if (N>>8) { k+=8; N>>=8; }
  if (N>>4) { k+=4; N>>=4; }
  if (N>>2) { k+=2; N>>=2; }
  if (N>>1) { k+=1; N>>=1; }
  return k ;
}

static void BitReverse(complex *xin, int N)
{
	int LH, i, j, k;
	struct complex tmp;

			LH=N/2;    
			j = N/2;

			for( i = 1; i <= (N -2); i++)
			{
				if(i < j)
				{
					tmp = xin[j];
					xin[j] = xin[i];
					xin[i] = tmp;
				}
			k = LH;
				while(j >= k)
				{
					j = j-k;
					k = k/2;
				}

			j = j + k;

			}

}

static void DFT_2(complex *b1, complex *b2)
{
struct complex tmp;
  tmp = *b1;

  (*b1).real = (*b1).real + (*b2).real;
  (*b1).imag = (*b1).imag + (*b2).imag;

  (*b2).real = tmp.real - (*b2).real;
  (*b2).imag = tmp.imag - (*b2).imag;	
}

static void DFT_4(complex* b0,  complex* b1, complex* b2, complex* b3)
{
  /*variables locales*/
  struct complex temp[4];
  
  /*calcul x1*/
  temp[0].real=(*b0).real+(*b1).real;		
  temp[0].imag=(*b0).imag+(*b1).imag;

  /*calcul x2*/
  temp[1].real=(*b0).real-(*b1).real;		
  temp[1].imag=(*b0).imag-(*b1).imag;

  /*calcul x3*/
  temp[2].real=(*b2).real+(*b3).real;	
  temp[2].imag=(*b2).imag+(*b3).imag;

  /*calcul x4 + multiplication with -j*/
  temp[3].imag=(*b3).real-(*b2).real;		
  temp[3].real=(*b2).imag-(*b3).imag; 

  /*the last stage*/
  (*b0).real=temp[0].real+temp[2].real;
  (*b0).imag=temp[0].imag+temp[2].imag;
  
  (*b1).real=temp[1].real+temp[3].real;     
  (*b1).imag=temp[1].imag+temp[3].imag; 
  
  (*b2).real=temp[0].real-temp[2].real;     
  (*b2).imag=temp[0].imag-temp[2].imag;
  
  (*b3).real=temp[1].real-temp[3].real; 
  (*b3).imag=temp[1].imag-temp[3].imag;
}



static void FFT_R4(complex *xin, int N, int m)
{
		int  i, L, j;
		double  ps1, ps2, ps3;
		int le,B;
		struct complex w[4];

		for( L = 1; L <= m; L++)
		{
			le = pow(4 ,L);
			B = le/4;       /*the distance of buttefly*/
		
 			for(j = 0; j <= B-1 ; j++)	
  			{
			       //  ps0 = (TWICEPI/N) * 0 * j;
			       //  w[0].real = cos(ps0);
			       //  w[0].imag = -sin(ps0);
	
				   ps1 = ((TWICEPI)/le)*2*j;
				   w[1].real = cos(ps1);
				   w[1].imag = -sin(ps1);

				   ps2 = (TWICEPI/le)*j;
				   w[2].real = cos(ps2);
				   w[2].imag = -sin(ps2);

				   ps3 = (TWICEPI/le)*3*j;
				   w[3].real = cos(ps3);
				   w[3].imag = -sin(ps3);

				   for(i = j; i <= N-1; i = i + le)	/* controle those same butteflies*/
				     {
						/* multiple with W */
				 //   xin[i] = multicomplex(xin[i], w[0]);
				      xin[i + B] = multicomplex(xin[i + B], w[1]);
				      xin[i + 2*B] = multicomplex(xin[i + 2*B], w[2]);
				      xin[i + 3*B] = multicomplex(xin[i + 3*B], w[3]);
						/* DFT-4 */
				      DFT_4(xin + i, xin + i + B, xin + i + 2*B, xin + i + 3*B);
				     }
  		      }
		/*
		printf("*****N°%d **********\n", L);
		for(i=0;i<N;i++)
		{
		printf("%.8f\t\t",xin[i].real);
		printf("%.8f\n",xin[i].imag);
		}
		*/
		}
	
}//end of FFT_R4

static void FFT_L2(complex *xin, int N)
{				/* For the last stage 2 DFT*/
	int j, B;
	double p, ps ;
	struct complex w;

		B = N/2;   			
		for(j = 0; j <= B - 1; j++)
		{			
			ps = (TWICEPI/N)*j;
		        w.real = cos(ps);
			w.imag = -sin(ps);

	    	 	/* multiple avec W */
            		 xin[j+ B] = multicomplex(xin[j + B], w);
			 DFT_2(xin + j ,xin + j + B);
		}
				
}//end of FFT_L2


/**************** FFT complex ***************/

 void RunFFT(complex *xin, int N)
{
	int m, i;
	BitReverse(xin, N);
	m = mylog2(N);
	if( (m%2) == 0 )
	{
		/*All the stages are radix 4*/
		FFT_R4(xin, N, m/2);
	}
	else 
	{
		/*the last stage is radix 2*/
		FFT_R4(xin, N, m/2);
		FFT_L2(xin, N);
	}
}

 void RunIFFT(complex *xin,int N)
{			/* inverse FFT */
      int i;
    
      for(i=0; i < N + 1 ; i++)
      { 
	xin[i].imag = -xin[i].imag;
      }

      RunFFT(xin,N);

      for(i = 0; i < N + 1 ; i++)
      { 
	xin[i].real = xin[i].real/N;
	xin[i].imag = -xin[i].imag/N;
      }
}

/**************     FFT real   ****************/

void RunFFTR(complex *xin, int N)
{
	int i;
	double ps;
	complex *Realin;
	complex Realtmp1;
	complex Realtmp2;
	complex w;

	Realin	= (complex *)malloc((N/2)*sizeof(complex));

			/*** X(n)= A(2n) +j*A(2n+1) ***/
	for(i = 0 ; i < N/2 ; i++)
	{
		Realin[i].real = xin[2*i].real;
		Realin[i].imag = xin[2*i + 1].real;
	}
	
	RunFFT(Realin, N/2);

	for(i = 0; i < N/2 ; i++)
	{
			/*****	factor w *****/
		ps = (TWICEPI/N)*i;
		w.real = cos(ps);
		w.imag = -sin(ps);

			/*****  conjugue *****/
	       Realtmp1.real = Realin[(N/2) - i].real;
	       Realtmp1.imag = -Realin[(N/2) - i].imag;	

		if(i == 0)
	       {
	       Realtmp1.real = Realin[0].real;
	       Realtmp1.imag = -Realin[0].imag;	
	       }	
			/*****	part 2 *****/ 
	       Realtmp2.real = Realin[i].imag - Realtmp1.imag;
	       Realtmp2.imag = Realtmp1.real - Realin[i].real;
	       if(i > 0)
	       Realtmp2 = multicomplex(Realtmp2, w);

			/****  part 1 ****/
	       Realtmp1.real = Realin[i].real + Realtmp1.real;	
	       Realtmp1.imag = Realin[i].imag + Realtmp1.imag;	
	
	       xin[i].real = (Realtmp1.real + Realtmp2.real)/2;
	       xin[i].imag = (Realtmp1.imag + Realtmp2.imag)/2;
	
	       xin[N/2 + i].real = (Realtmp1.real - Realtmp2.real)/2;
	       xin[N/2 + i].imag = (Realtmp1.imag - Realtmp2.imag)/2;
			
	}

}

void RunIFFTR(complex *xin, int N)
{

	int i;
	double ps;
	complex *Realin;
	complex Realtmp1;
	complex Realtmp2;
	complex w;

	Realin	= (complex *)malloc((N/2)*sizeof(complex));

			/*** X(n)= A(2n) +j*A(2n+1) ***/
	for(i = 0 ; i < N/2 ; i++)
	{
		Realin[i].real = xin[2*i].real;
		Realin[i].imag = xin[2*i + 1].real;
	}
	
	RunIFFT(Realin, N/2);

	for(i = 0; i < N/2 ; i++)
	{
			/*****	factor w *****/
		ps = (TWICEPI/N)*i;
		w.real = cos(ps);
		w.imag = -sin(ps);

			/*****  conjugue *****/
	       Realtmp1.real = Realin[(N/2) - i].real;
	       Realtmp1.imag = -Realin[(N/2) - i].imag;	

		if(i == 0)
	       {
	       Realtmp1.real = Realin[0].real;
	       Realtmp1.imag = -Realin[0].imag;	
	       }	
			/*****	part 2 *****/ 
	       Realtmp2.real = Realin[i].imag - Realtmp1.imag;
	       Realtmp2.imag = Realtmp1.real - Realin[i].real;
	       if(i > 0)
	       Realtmp2 = multicomplex(Realtmp2, w);

			/****  part 1 ****/
	       Realtmp1.real = Realin[i].real + Realtmp1.real;	
	       Realtmp1.imag = Realin[i].imag + Realtmp1.imag;	
	
	       xin[i].real = (Realtmp1.real + Realtmp2.real)/2;
	       xin[i].imag = (Realtmp1.imag + Realtmp2.imag)/2;
	
	       xin[N/2 + i].real = (Realtmp1.real - Realtmp2.real)/2;
	       xin[N/2 + i].imag = (Realtmp1.imag - Realtmp2.imag)/2;
			
	}
	
}

