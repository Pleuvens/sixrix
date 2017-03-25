# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>
# include <err.h>
# include "fft.c"
# include "wave.h"
# define PI 3.14159265358979323846

struct HEADER header;

void traitement(char *k) {  
  char *filename = (char*) malloc(sizeof(char) * 1024);
  unsigned char buffer4[4];
  unsigned char buffer2[2];

  FILE *ptr = fopen(k, "rb");
  if (ptr == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int read = 0;

  // read header parts

  read = fread(header.riff, sizeof(header.riff), 1, ptr); 

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
   // convert little endian to big endian 4 byte int
  header.overall_size  = buffer4[0] | 
    (buffer4[1]<<8) | 
    (buffer4[2]<<16) | 
    (buffer4[3]<<24);

  printf("Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

  read = fread(header.wave, sizeof(header.wave), 1, ptr);
  printf("Wave marker: %s\n", header.wave);

  read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
  printf("Fmt marker: %s\n", header.fmt_chunk_marker);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  // convert little endian to big endian 4 byte integer
  header.length_of_fmt = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) |
    (buffer4[3] << 24);
  printf("Length of Fmt header: %u \n", header.length_of_fmt);

  read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u \n", buffer2[0], buffer2[1]);

  header.format_type = buffer2[0] | (buffer2[1] << 8);
  char format_name[10] = "";
  if (header.format_type == 1)
    strcpy(format_name,"PCM"); 
  else if (header.format_type == 6)
    strcpy(format_name, "A-law");
  else if (header.format_type == 7)
    strcpy(format_name, "Mu-law");

  printf("Format type: %u %s \n", header.format_type, format_name);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);
  printf("%u %u \n", buffer2[0], buffer2[1]);

  header.channels = buffer2[0] | (buffer2[1] << 8);
  printf("Channels: %u \n", header.channels);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  header.sample_rate = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) |
    (buffer4[3] << 24);

  printf("Sample rate: %u\n", header.sample_rate);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  header.byterate  = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) |
    (buffer4[3] << 24);
  printf("Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);
  printf("%u %u \n", buffer2[0], buffer2[1]);

  header.block_align = buffer2[0] |
    (buffer2[1] << 8);
  printf("Block Alignment: %u \n", header.block_align);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);
  printf("%u %u \n", buffer2[0], buffer2[1]);

  header.bits_per_sample = buffer2[0] |
    (buffer2[1] << 8);
  printf("Bits per sample: %u \n", header.bits_per_sample);

  read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
  printf("Data Marker: %s \n", header.data_chunk_header);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

  header.data_size = buffer4[0] |
    (buffer4[1] << 8) |
    (buffer4[2] << 16) | 
    (buffer4[3] << 24 );
  printf("Size of data chunk: %u \n", header.data_size);


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
      long i =0;
      char data_buffer[size_of_each_sample];
      int  size_is_correct = TRUE;
      long bytes_in_each_channel = (size_of_each_sample / header.channels);
      if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
	size_is_correct = FALSE;
      }
      if (size_is_correct) { 				
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
	printf("\nOutput Hanning : ");
	 for(long i = 0; i < 50; ++i)
	   printf("%lf ", tab[i]);
	printf("\n");

//	printf("\nTaille des données restantes %zu \n", k);

	fft(second, k, PI);
	printf("\n");
	print_result("Output FFT : ", second, 50);
	printf("\n");
	unsigned long tableau[k];
	for(long i = 0; i < k; ++i){
	  tableau[i] = csqrtl(pow(creal(second[i]), 2)+ pow(cimag(second[i]), 2));
	  while(tableau[i] > 127)
	    tableau[i] /= 127;
	 }
	 printf("Final Output : ");
	 for(long i = 0; i < 50; ++i)
	   printf("%lu ", tableau[i]);
	printf("\n");
      } 
  fclose(ptr);
  free(filename);
}
