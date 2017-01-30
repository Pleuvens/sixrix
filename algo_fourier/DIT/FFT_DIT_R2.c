/***********************************
// DIT radix-2  FFT  complex 
//	for calculer 4, 8, 16, 32, 64, 128, 256... points FFT  
//
// 18 juillet 2007
// purcharse*gmail.com
************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct complex
{ double real;
  double imag;
} complex ;

struct complex multicomplex(struct complex,struct complex);
int mylog2(int N);
void DFT_2(struct complex *,struct complex *);
//void DFT_4(struct complex *,struct complex *,struct complex *,struct complex *);

void reverse(struct complex *, int);
void FFT(struct complex *,int);
void IFFT(struct complex *,int);

float  result[257];
struct  complex s[257];
int   Num= 8;
const float PI=3.14159;


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
     */

    /*
	s[0].real = 10;
	s[0].imag = 10;
	
	/*
      for(i=Num/2-5;i<Num/2+5;i++)
      {
          s[i].real=10;
          s[i].imag=0;
      }
	*/
	/*  sinus*/
	
      for(i=0;i<Num+1;i++)
      {
          s[i].real=sin(PI*i/Num);
          s[i].imag=cos(PI*i/Num);
      } 
	
	/*  */
	/*
      for(i=0;i<Num+1;i++)
      {
          s[i].real=0;
          s[i].imag=0;
      }
      for(i=Num*3/8;i<Num/2;i++)
      {
          s[i].real=(i-Num*3/8);
          s[i].imag=0;
      }
      for(i=Num/2;i<Num*5/8;i++)
      {
          s[i].real=(Num*5/8-i);
          s[i].imag=0;
      }       */   

	printf("*********** donnees *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%.8f\t\t",s[i].real);
		printf("%.8f\n",s[i].imag);
	}

	FFT(s,Num);

	printf("*********** FFT *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%.8f\t\t",s[i].real);
		printf("%.8f\n",s[i].imag);
	}

	IFFT(s, Num);
	printf("*********** IFFT *****************\n");
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


int mylog2(int N) 
{
  int k;
  for( k = 1; (N = N/2) != 1; k++) {;}  
  return k;
}

void reverse(struct complex *xin, int N)
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

void DFT_2(struct complex *b1,struct complex *b2)
{
struct complex tmp;
  tmp = *b1;

  (*b1).real = (*b1).real + (*b2).real;
  (*b1).imag = (*b1).imag + (*b2).imag;

  (*b2).real = tmp.real - (*b2).real;
  (*b2).imag = tmp.imag - (*b2).imag;	
}



void FFT(struct complex *xin, int N)
{
	int i, j, m, L;
	double p, ps ;
	int le, B;
	struct complex w;

	m = mylog2(N);
	reverse(xin, N);

	for(L = 1; L <= m; L++)
	{
		le = pow(2,L); /* le longeur de papillon */
		B = le/2;    /* l'eclat des papillons  */		
	
		for(j = 0; j <= B - 1; j++)
		{
			p = pow(2, m - L)*j;				
			ps = (2*PI/N)*p;
		        w.real = cos(ps);
			w.imag = -sin(ps);
			
			for(i = j; i <= N - 1; i = i + le)
			{
	    	 		/* multiple avec W */
            		      xin[i + B] = multicomplex(xin[i + B], w);
			      DFT_2(xin + i, xin + i + B);			

			}	
		}
		/*
		printf("*****N°%d **********\n", L);
		for(i=0;i<Num;i++)
		{
		printf("%.8f\t\t",xin[i].real);
		printf("%.8f\n",xin[i].imag);
		}		
			*/
	}	
	
}//fin du FFT


void IFFT(struct complex *xin,int N)
{

      int i;
    
      for(i=0; i < Num + 1 ; i++)
      { 
	xin[i].imag = -xin[i].imag;
      }
   
      FFT(xin,N);

      for(i = 0; i < Num + 1 ; i++)
      { 
	xin[i].real = xin[i].real/Num;
	xin[i].imag = -xin[i].imag/Num;

      }
}



