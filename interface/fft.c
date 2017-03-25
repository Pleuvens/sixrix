# include <stdio.h>
# include <math.h>
# include <complex.h>

typedef double complex cplx;

// Algo de la fft

void _fft(cplx tab[], cplx tmp[], int n, int step, double PI)
{
  if (step < n) {
    _fft(tmp, tab, n, step * 2, PI);
    _fft(tmp + step, tab + step, n, step * 2, PI);
    for (int i = 0; i < n; i += 2 * step) {
      cplx t = cexp(-I * PI * i / n) * tmp[i + step];
      tab[i / 2]     = tmp[i] + t;
      tab[(i + n)/2] = tmp[i] - t;
    }
  }
}

// Fonction d'appel (crée le tableau de resultat)

void fft(cplx tab[], int n, double PI)
{
  cplx tmp[n];
  for (int i = 0; i < n; i++) 
    tmp[i] = tab[i];
  _fft(tab, tmp, n, 1, PI);
}

// Fonction de print qui affiche sous forme de couple (reel, imaginaire) un tableau de cplx 

void print_result(const char * s, cplx buf[], unsigned long k) {
  printf("%s", s);
  for (unsigned long i = 0; i < k; i++){
    printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
  }
  printf("\n");
}
