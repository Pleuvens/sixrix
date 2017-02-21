#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "math_hanning.h"
#define PI 3.14159265358979323846

void hanning1(double tab[],size_t len){

  for (size_t i = 0; i < len; i++) {
      double mul= 0.5 * (1 - cos(2*PI*i/len-1));
      tab[i]=mul*tab[i];
  }

}

float *hanning(int N, short itype)
{
    int half, i, idx, n;
    float *w;

    w = (float*) calloc(N, sizeof(float));
    memset(w, 0, N*sizeof(float));

    if(itype==1)    //periodic function
        n = N-1;
    else
        n = N;

    if(n%2==0)
    {
        half = n/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (1 - cos(2*PI*(i+1) / (n+1)));

        idx = half-1;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }
    else
    {
        half = (n+1)/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (1 - cos(2*PI*(i+1) / (n+1)));

        idx = half-2;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }

    if(itype==1)    //periodic function
    {
        for(i=N-1; i>=1; i--)
            w[i] = w[i-1];
        w[0] = 0.0;
    }
    return(w);
}




int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  double dataIn[10];
  for (size_t i = 0; i <10; i++) {
    dataIn[i]=1;
    printf(" %f ",dataIn[i]);
  }
  printf("\n");
  printf("hanning\n");

  hanning1(dataIn,10);

  for (size_t i = 0; i <10; i++) {
    printf(" %f ",dataIn[i]);
  }
  printf("\n");

  //float data[10];
  float *data=hanning(15,1);







  return 0;
}
