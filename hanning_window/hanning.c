#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

double *hanning(double tabIn[],size_t len){

  double *dataOut=malloc(len*sizeof(double));
  memset(dataOut,0,len*sizeof(double));

  for (size_t i = 0; i < len; i++) {
    double mul = 0.5 * (1 - cos(2*PI*i/len-1));
    dataOut[i] = mul * tabIn[i];
  }
  return dataOut;
}


int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

//*TEST
  double dataIn[10]={0};
  for (size_t i = 0; i <10; i++) {
    dataIn[i]=2;
    printf(" %f ",dataIn[i]);
  }
  printf("\n");
  printf("Hanning Result :\n");

  double *dataOut=hanning(dataIn,10);

  for (size_t i = 0; i <10; i++) {
    printf(" %lf ",dataOut[i]);
  }
  printf("\n");

  free(dataOut);
//*/
  return 0;
}
