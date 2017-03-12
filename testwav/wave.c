# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>
# include <err.h>
# include "fft.c"
# include "wave.h"
# define TRUE 1 
# define FALSE 0
# define PI 3.14159265358979323846

struct HEADER header;

int main(int argc, char **argv) {
  (void)argc;  
  char *filename = (char*) malloc(sizeof(char) * 1024);
  unsigned char buffer4[4];
  unsigned char buffer2[2];

  // chemin du fichier
  char cwd[1024];
  if(getcwd(cwd, sizeof(cwd)) == NULL)
  	err(3, "fail on getcwd");
  strcpy(filename, cwd);
  strcat(filename, "/");
  strcat(filename, argv[1]);

  FILE *ptr = fopen(filename, "rb");
  if (ptr == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int read = 0;

  // read header parts

  read = fread(header.riff, sizeof(header.riff), 1, ptr);
  printf("(1-4): %s \n", header.riff); 

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  // convert little endian to big endian 4 byte int
  header.overall_size  = buffer4[0] | 
    (buffer4[1]<<8) | 
    (buffer4[2]<<16) | 
    (buffer4[3]<<24);

  printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

  read = fread(header.wave, sizeof(header.wave), 1, ptr);
  printf("(9-12) Wave marker: %s\n", header.wave);

  read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
  printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  // convert little endian to big endian 4 byte integer
  header.length_of_fmt = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) |
    (buffer4[3] << 24);
  printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

  read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u \n", buffer2[0], buffer2[1]);

  header.format_type = buffer2[0] | (buffer2[1] << 8);
  char format_name[10] = "";
  if (header.format_type == 1)
    strcpy(format_name,"PCM"); 
  else if (header.format_type == 6)
    strcpy(format_name, "A-law");
  else if (header.format_type == 7)
    strcpy(format_name, "Mu-law");

  printf("(21-22) Format type: %u %s \n", header.format_type, format_name);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);
  printf("%u %u \n", buffer2[0], buffer2[1]);

  header.channels = buffer2[0] | (buffer2[1] << 8);
  printf("(23-24) Channels: %u \n", header.channels);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  header.sample_rate = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) |
    (buffer4[3] << 24);

  printf("(25-28) Sample rate: %u\n", header.sample_rate);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  header.byterate  = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) |
    (buffer4[3] << 24);
  printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);
  printf("%u %u \n", buffer2[0], buffer2[1]);

  header.block_align = buffer2[0] |
    (buffer2[1] << 8);
  printf("(33-34) Block Alignment: %u \n", header.block_align);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);
  printf("%u %u \n", buffer2[0], buffer2[1]);

  header.bits_per_sample = buffer2[0] |
    (buffer2[1] << 8);
  printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);

  read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
  printf("(37-40) Data Marker: %s \n", header.data_chunk_header);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  header.data_size = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) | 
    (buffer4[3] << 24 );
  printf("(41-44) Size of data chunk: %u \n", header.data_size);


  // calculate no.of samples
  long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
  printf("Number of samples:%lu \n", num_samples);

  long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
  printf("Size of each sample:%ld bytes\n", size_of_each_sample);

  // durée du fichier
  float duration_in_seconds = (float) header.overall_size / header.byterate;
  printf("Approx.Duration in seconds=%f\n", duration_in_seconds);



  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  double tab[num_samples];
  if (header.format_type == 1) {
    printf("Continuer? Y/N?");
    char c = 'n';
    if(scanf("%c", &c) == -1)
    	err(3, "fail on scanf");
    if (c == 'Y' || c == 'y') {
      long i =0;
      char data_buffer[size_of_each_sample];
      int  size_is_correct = TRUE;
      long bytes_in_each_channel = (size_of_each_sample / header.channels);
      if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
	size_is_correct = FALSE;
      }
      if (size_is_correct) { 
/*	long low_limit = 0l;
	long high_limit = 0l;

	switch (header.bits_per_sample) {
	  case 8:
	    low_limit = -128;
	    high_limit = 127;
	    break;
	  case 16:
	    low_limit = -32768;
	    high_limit = 32767;
	    break;
	  case 32:
	    low_limit = -2147483648;
	    high_limit = 2147483647;
	    break;
	}*/					
	for (i =1; i <= num_samples; i++) {
	  read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
	  if (read == 1) {
	    unsigned int  xchannels = 0;
	    int data_in_channel = 0;
	    for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {
	      // passage des petits endian en grand endian
	      if (bytes_in_each_channel == 4) {
		data_in_channel = data_buffer[0] | 
		  (data_buffer[1]<<8) | 
		  (data_buffer[2]<<16) | 
		  (data_buffer[3]<<24);
	      }
	      else if (bytes_in_each_channel == 2) {
		data_in_channel = data_buffer[0] |
		  (data_buffer[1] << 8);
	      }
	      else if (bytes_in_each_channel == 1) {
		data_in_channel = data_buffer[0];
	      }
	      tab[i] = (data_in_channel * 0.5 * (1-cos(2*PI*i/(sizeof(tab)/sizeof(double))-1)));
	    }
	  }
	}
	long k = 1;
	while ((k*2) < num_samples){
	  k *= 2;
	}
	cplx second[k];
	for(long i = 0; i < k; ++i)
	  second[i] = tab[i];

//	printf("\nTaille des données restantes %zu \n", k);

	fft(second, k, PI);

	//print_result("Output : ", second, k);

	unsigned long tableau[k];
	for(long i = 0; i < k; ++i){
	  tableau[i] = csqrtl(pow(creal(second[i]), 2)+ pow(cimag(second[i]), 2));
	  while(tableau[i] > 127)
	    tableau[i] /= 127;
	  printf("%lu ", tableau[i]);
	}
	printf("\n");
      } 
    }
  } 
  fclose(ptr);
  free(filename);
  return 0;
}
