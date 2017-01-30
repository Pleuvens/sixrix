/***********************************
// DIT radix-4  FFT  complex 
//	for calculer 4, 16, 64, 256, 1024 ... points FFT  
//
// 18 juillet 2007
// purcharse*gmail.com
************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct complex
{ float real;
  float imag;
} complex ;

struct complex multicomplex(struct complex,struct complex);
int mylog4(int N);

void DFT_4(struct complex *,struct complex *,struct complex *,struct complex *);
void DFT_2(struct complex *,struct complex *);

void reverse(struct complex *, int);
void FFT(struct complex *,int);
void IFFT(struct complex *,int);

float  result[257];
struct  complex s[257];
int   Num = 16;
const float PI=3.14159;

main()
{
	int i;
	struct complex w,resultat;

	/* rectangle */
     
      for(i=0; i<Num ;i++)
      {
          s[i].real=0;
          s[i].imag=0;
      }
	
	/*
      for(i=Num/2-5;i<Num/2+5;i++)
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
          s[i].imag=cos(PI*i/Num);
      } 
	*/
	/*  */
	/*
      for(i=0;i<Num+1;i++)
      {
          s[i].real=0;
          s[i].imag=0;
      }*/
	/*
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

	s[0].real = 2768;
	s[1].real = 4840;
	s[2].real = 3472;
	s[3].real = 1200;
	s[4].real = 3360;
	s[5].real = 1568;
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
		printf("%f\t\t",s[i].real);
		printf("%f\n",s[i].imag);
	}

	FFT(s,Num);
	printf("*********** FFT *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%f\t\t",s[i].real);
		printf("%f\n",s[i].imag);
	}

	IFFT(s,Num);
	printf("*********** IFFT *****************\n");
	for(i=0;i<Num;i++)
	{
		printf("%f\t\t",s[i].real);
		printf("%f\n",s[i].imag);
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

int mylog4(int N) 
{
  int k;
  for( k = 1; (N = N/4) != 1; k++) {;}  
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

void DFT_4(struct complex* b0, struct complex* b1, struct complex* b2,struct complex* b3)
{
  /*variables locales*/
  struct complex temp[4];
  
  /*calcul y1*/
  temp[0].real=(*b0).real+(*b1).real;		
  temp[0].imag=(*b0).imag+(*b1).imag;

  /*calcul y2*/
  temp[1].real=(*b0).real-(*b1).real;		
  temp[1].imag=(*b0).imag-(*b1).imag;

  /*calcul y3*/
  temp[2].real=(*b2).real+(*b3).real;	
  temp[2].imag=(*b2).imag+(*b3).imag;

  /*calcul y4 + multiplication par -j*/
  temp[3].imag=(*b3).real-(*b2).real;		
  temp[3].real=(*b2).imag-(*b3).imag; 

  /*dernier etage*/
  (*b0).real=temp[0].real+temp[2].real;
  (*b0).imag=temp[0].imag+temp[2].imag;
  
  (*b1).real=temp[1].real+temp[3].real;     
  (*b1).imag=temp[1].imag+temp[3].imag; 
  
  (*b2).real=temp[0].real-temp[2].real;     
  (*b2).imag=temp[0].imag-temp[2].imag;
  
  (*b3).real=temp[1].real-temp[3].real; 
  (*b3).imag=temp[1].imag-temp[3].imag;
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

		int  m, i, j, L;
		double  ps1, ps2, ps3, p ;
		int le,B;
		struct complex w[4];

	m = mylog4(N);
	reverse(xin, N);

		for(L = 1; L <= m; L++)
		{
			le = pow(4 ,L); /*le longeur de papillon*/
			B = le/4;       /*l'eclat des papillons*/
		
			p = pow(4,m-L);

 			for(j = 0; j <= B-1 ; j++)	/* controle	*/
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
				     
				   for(i = j; i <= N-1; i = i + le)	/* controle those same buttefly*/
				     {
					/* multiple avec W */
				 //   xin[i] = multicomplex(xin[i], w[0]);
				      xin[i + B] = multicomplex(xin[i + B], w[1]);
				      xin[i + 2*B] = multicomplex(xin[i + 2*B], w[2]);
				      xin[i + 3*B] = multicomplex(xin[i + 3*B], w[3]);
						/* DFT-4 */
				      DFT_4(xin + i, xin + i + B, xin + i + 2*B, xin + i + 3*B);
				     }
  		      }
				

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

