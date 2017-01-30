/**********************************
// DIF radix-2  FFT complex et FFT real
//
// 12 juillet 2007
// purcharse*gmail.com
***********************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define FFT_forward 1
#define FFT_inverse 0

struct complex
{ 
  double real;
  double imag;
} complex ;

struct complex multicomplex(struct complex,struct complex);
void DFT_2(struct complex *,struct complex *);

void reverse(struct complex *, int);

void FFT(struct complex *,int, int);
void FFTR(struct complex *, int);

float  result[257];
struct  complex s[257];
int   Num= 8;
const float PI=3.14159;

/*****************main programe********************/
main()
{
	int i;


	/* rectangle */
      /*
      for(i=0;i<Num+1;i++)
      {
          s[i].real=0;
          s[i].imag=0;
      }

	s[0].real = 10;
	s[0].imag = 10;
	*/
	/*
      for(i=Num/2-10;i<Num/2+10;i++)
      {
          s[i].real=10;
          s[i].imag=0;
      }
	*/
	/*  sinus*/
	/*	
      for(i=0;i<Num+1;i++)
      {
          s[i].real=sin(PI*i/Num);
          s[i].imag= 0;
      } 
	*/

	
     for(i = 0; i < Num+1 ; i++)
      {
          s[i].real=0;
          s[i].imag=0;
      }

  s[0].real =  2768;
  s[1].real =  4840;
  s[2].real =  3472;
  s[3].real =  1200;
  s[4].real =  3360;
  s[5].real =  1568;
  s[6].real = -2200;
  s[7].real = -2464;
  s[8].real = -3360;
  s[9].real = -7056;
  s[10].real = -6800;
  s[11].real = -6688;
  s[12].real = -7960;
  s[13].real = -5072;
  s[14].real = -4272;
  s[15].real = -5400;

	printf("*********** donnees *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%.8f\t\t",s[i].real);
		printf("%.8f\n",s[i].imag);
	}

	//FFT(s,Num,FFT_inverse);
	FFTR(s,Num);
	printf("********** FFTR *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%.8f\t\t",s[i].real);
		printf("%.8f\n",s[i].imag);
	}
	

	

}

/***************** function ********************/

struct complex multicomplex(struct complex b1,struct complex b2)  /* multiplication des complex */
{
struct complex b3;
b3.real=b1.real*b2.real-b1.imag*b2.imag;
b3.imag=b1.real*b2.imag+b1.imag*b2.real;
return(b3);
}

void DFT_2(struct complex *b1,struct complex *b2)
{
struct complex tmp;
  tmp = *b1;

  (*b1).real = (*b1).real + (*b2).real;
  (*b1).imag = (*b1).imag + (*b2).imag;

  (*b2).real = tmp.real - (*b2).real;
  (*b2).imag = tmp.imag - (*b2).imag;	
}

void reverse(struct complex *xin, int N)
{
	int LH, i, j, k;
	struct complex tmp;	
			/* bit reverse*/
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

void FFT(struct complex *xin,int N, int direction)
{
		int i, j, f, m, L;
		double p, ps ;
		int le, B;
		struct complex w;
		/* 2^m = N   m = log2 N */
	f = N;

	for( m = 1; (f = f/2) != 1; m++) {;}
	
		for(L = m; L >= 1; L--)  
		{ 
			le = pow(2,L);
			B = le/2;      /*Distance of buttefly*/
		
			 p = pow(2,m-L);

 			for(j = 0; j <= B-1 ; j++)
  			{  
				   ps = (2*PI/N)*p*j;

				   w.real = cos(ps);
				   w.imag = -sin(ps);
				  
				  if(direction == FFT_inverse)
				   w.imag = sin(ps);
				 
				   for(i = j; i <= N-1; i = i + le)
				     {
						/* DFT-2 */
				      DFT_2(xin + i, xin + i + B);			
    						/* multiple with W */
				      xin[i + B] = multicomplex(xin[i + B], w);
				     }
  		      }

		}

		reverse(xin, N);

	   if(direction == FFT_inverse)
	   for(i = 0; i<N;i++)
  	  {
          	xin[i].real /= N;
          	xin[i].imag /= N;
      	  }
} 

void FFTR(struct complex *xin, int N)
{
	unsigned int i;
	double ps,p;
	struct complex *Realin;
	struct complex Realtmp1;
	struct complex Realtmp2;
	struct complex w;

	Realin	= (struct complex *)malloc((N/2)*sizeof(struct complex));

			/*** X(n)= A(2n) +j*A(2n+1) ***/
	for(i = 0 ; i<= N/2 -1 ; i++)
	{
		Realin[i].real = xin[2*i].real;
		Realin[i].imag = xin[2*i + 1].real;
	}
	
	FFT(Realin, N/2, FFT_forward);


	for(i = 0; i<= N/2 -1 ; i++)
	{
			//  factor w 
	 	ps = (2*PI/N)*i;
		
		 w.real = cos(ps);
		 w.imag = -sin(ps);
			//  conjuguate
		if(i == 0)
	       {
	       Realtmp1.real = Realin[0].real;
	       Realtmp1.imag = -Realin[0].imag;	
	       }
	       else
	       {
	       Realtmp1.real = Realin[(N/2) - i].real;
	       Realtmp1.imag = -Realin[(N/2) - i].imag;	
	       }
			//  part 2  
	       Realtmp2.real = Realin[i].imag - Realtmp1.imag;
	       Realtmp2.imag = Realtmp1.real - Realin[i].real;

	       Realtmp2 = multicomplex(Realtmp2, w);

			//  part 1 
	       Realtmp1.real = Realin[i].real + Realtmp1.real;	
	       Realtmp1.imag = Realin[i].imag + Realtmp1.imag;	
	
	       xin[i].real = (Realtmp1.real + Realtmp2.real)/2;
	       xin[i].imag = (Realtmp1.imag + Realtmp2.imag)/2;
	
	       xin[N/2 + i].real = (Realtmp1.real - Realtmp2.real)/2;
	       xin[N/2 + i].imag = (Realtmp1.imag - Realtmp2.imag)/2;	
    }

}


