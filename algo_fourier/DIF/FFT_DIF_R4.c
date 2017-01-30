/**********************************
// DIF radix-4  
//	FFT complex and FFT complex
//	IFFT real and IFFT real
//
//	The function special for FFT real
//
// 12 juillet 2007
//purcharse*gmail.com
***********************************/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define FFT_forward 1
#define FFT_inverse 0

typedef struct complex
{ double real;
  double imag;
} complex ;

struct complex multicomplex( complex, complex);
void DFT_4_F( complex *,complex *, complex *, complex *);
void DFT_4_R( complex *, complex *, complex *, complex *);
void reverse( complex *, int);
void FFT( complex *,int,int);
void FFTR( complex *, int);
void IFFTR_special( complex *, int);

float  result[257];
struct  complex s[257];
int   Num = 8;
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
	
     /* for(i=0;i<Num;i++)
      {
          s[i].real=sin(PI*i/Num);
          s[i].imag=0;
      } */

		for( i = 0; i< Num; i++)
		s[i].imag = 0 ;

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

	FFTR(s,Num);
	
	printf("*********** FFT *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%.8f\t\t",s[i].real);
		printf("%.8f\n",s[i].imag);
	}
	

	/*IFFTR_special(s, Num);
	printf("*********** IFFT *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%.8f\t\t",s[i].real);
		printf("%.8f\n",s[i].imag);
	}
	*/
}

/***************** function ********************/

 complex multicomplex( complex b1, complex b2)  /* multiplication des complex */
{
 complex b3;
b3.real=b1.real*b2.real-b1.imag*b2.imag;
b3.imag=b1.real*b2.imag+b1.imag*b2.real;
return(b3);
}

void DFT_4(complex* b0,complex* b1,  complex* b2, complex* b3)
{
  /*variables locales*/
  struct complex temp[4];
  
  /*calcul y1*/
  temp[0].real=(*b0).real+(*b2).real;		
  temp[0].imag=(*b0).imag+(*b2).imag;

	
  /*calcul y2*/
  temp[1].real=(*b1).real+(*b3).real;		
  temp[1].imag=(*b1).imag+(*b3).imag;

  /*calcul y3*/
  temp[2].real=(*b0).real-(*b2).real;	
  temp[2].imag=(*b0).imag-(*b2).imag;

  /*calcul y4 + multiplication par -j*/
  temp[3].imag=(*b3).real-(*b1).real;		
  temp[3].real=(*b1).imag-(*b3).imag; 

  /*dernier etage*/
  (*b0).real=temp[0].real+temp[1].real;
  (*b0).imag=temp[0].imag+temp[1].imag;
  
  (*b1).real=temp[0].real-temp[1].real;     
  (*b1).imag=temp[0].imag-temp[1].imag; 
  
  (*b2).real=temp[2].real+temp[3].real;     
  (*b2).imag=temp[2].imag+temp[3].imag;
  
  (*b3).real=temp[2].real-temp[3].real; 
  (*b3).imag=temp[2].imag-temp[3].imag; 
}

void DFT_4_F(complex* b0,complex* b1,  complex* b2,complex* b3)
{
  /*variables locales*/
  struct complex temp[4];
  
  /*calcul y1*/
  temp[0].real = (*b0).real + (*b1).real + (*b2).real + (*b3).real;		
  temp[0].imag = (*b0).imag + (*b1).imag + (*b2).imag + (*b3).imag;
	
  /*calcul y2*/
  temp[1].real = (*b0).real - (*b1).real + (*b2).real - (*b3).real;	
  temp[1].imag = (*b0).imag - (*b1).imag + (*b2).imag - (*b3).imag;

  temp[2].real = (*b0).real +(*b1).imag -(*b2).real - (*b3).imag;		
  temp[2].imag = (*b0).imag -(*b1).real -(*b2).imag + (*b3).real;

  /*calcul y4*/
  temp[3].real = (*b0).real -(*b1).imag - (*b2).real + (*b3).imag;		
  temp[3].imag = (*b0).imag +(*b1).real - (*b2).imag - (*b3).real; 
  
  *b0 = temp[0];
  *b1 = temp[1];
  *b2 = temp[2];
  *b3 = temp[3];
}

void DFT_4_I(complex* b0, complex* b1,  complex* b2,complex* b3)
{
  /*variables locales*/
  complex temp[4];
  
  /*calcul y1*/
  temp[0].real = (*b0).real + (*b1).real + (*b2).real + (*b3).real;		
  temp[0].imag = (*b0).imag + (*b1).imag + (*b2).imag + (*b3).imag;
	
  /*calcul y2*/
  temp[1].real = (*b0).real - (*b1).real + (*b2).real - (*b3).real;	
  temp[1].imag = (*b0).imag - (*b1).imag + (*b2).imag - (*b3).imag;

  temp[2].real = (*b0).real -(*b1).imag -(*b2).real + (*b3).imag;		
  temp[2].imag = (*b0).imag +(*b1).real -(*b2).imag - (*b3).real;

  /*calcul y4*/
  temp[3].real = (*b0).real +(*b1).imag - (*b2).real - (*b3).imag;		
  temp[3].imag = (*b0).imag -(*b1).real - (*b2).imag + (*b3).real; 
  
  *b0 = temp[0];
  *b1 = temp[1];
  *b2 = temp[2];
  *b3 = temp[3];
}

void reverse(complex *xin, int N)
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

void FFT( complex *xin,int N, int direction)
{
		int f, m, i, j, L;
		double  ps1, ps2, ps3, p ;
		int le,B;
		complex w[4];

		/* 4^m=N   m = log4 N */
	f = N;
	for(m = 1; (f = f/4) != 1; m++ ) {;}  

		
		for(L = m; L >= 1; L--)  
		{ 
			le = pow(4 ,L); /*le longeur de papillon*/
			B = le/4;       /*l'eclat des papillons*/
		
			p = pow(4,m-L);

 			for(j = 0; j <= B-1 ; j++)
  			{
			       //  ps0 = (2*pp/N) * 0 * j;
			       //  w[0].real = cos(ps0);
			       //  w[0].imag = -sin(ps0);

				   ps1 = (2*PI/N)*p*2*j; 
				   w[1].real = cos(ps1);
				   w[1].imag = -sin(ps1);

				   ps2 = (2*PI/N)*p*1*j;
				   w[2].real = cos(ps2);
				   w[2].imag = -sin(ps2);

				   ps3 = (2*PI/N)*p*3*j;
				   w[3].real = cos(ps3);
				   w[3].imag = -sin(ps3);

				if(direction == FFT_inverse)
				{
					w[1].imag = sin(ps1);
					w[2].imag = sin(ps2);
					w[3].imag = sin(ps3);
				}

				   for(i = j; i <= N-1; i = i + le)
				     {
						/* DFT-4 */
				       
					if(direction == FFT_inverse)
			        	DFT_4_I(xin + i, xin + i + B, xin + i + 2*B, xin + i + 3*B);
					else
					DFT_4(xin + i, xin + i + B, xin + i + 2*B, xin + i + 3*B);

    						/* multiple avec W */
				 //   xin[i] = multicomplex(xin[i], w[0]);
				      xin[i + B] = multicomplex(xin[i + B], w[1]);
				      xin[i + 2*B] = multicomplex(xin[i + 2*B], w[2]);
				      xin[i + 3*B] = multicomplex(xin[i + 3*B], w[3]);
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
	
} /* fin FFT */

void FFTR( complex *xin, int N)
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
	
	FFT(Realin, N/2, FFT_forward);

	for(i = 0; i < N/2 ; i++)
	{
			/*****	factor w *****/
		ps = (2*PI/N)*i;
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

void IFFTR_special( complex *xin, int N)
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
	
	FFT(Realin, N/2, FFT_inverse);

	for(i = 0; i < N/2 ; i++)
	{
			/*****	factor w(-k) *****/
		ps = (2*PI/N)*i;
		w.real = cos(ps);
		w.imag = sin(ps);

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
