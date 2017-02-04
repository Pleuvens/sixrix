#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI 3.14159265358979323846

void hanning(double tab[],size_t len){

  for (size_t i = 0; i < len; i++) {
      double mul= 0.5 * (1 - cos(2*PI*i/len-1));
      tab[i]=mul*tab[i];
  }

}

int main(int argc, char const *argv[]) {
(void)argc;
(void)argv;

//*  Test
double dataIn[10];
for (size_t i = 0; i <10; i++) {
  dataIn[i]=1;
  printf(" %f ",dataIn[i]);
}
printf("hanning\n");
hanning(dataIn,10);

for (size_t i = 0; i <10; i++) {
    printf(" %f ",dataIn[i]);
}
//*/


  return 0;
}
