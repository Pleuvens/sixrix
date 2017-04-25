# ifndef MFCC_H
# define MFCC_H

# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>
# include <err.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "../phonemes/fft.c"

# define PI 3.14159265358979323846
# define true 1
# define false 0

// WAVE file header format
struct HEADER {
    unsigned char riff[4];                      // RIFF string
    unsigned int overall_size   ;               // overall size of file in bytes
    unsigned char wave[4];                      // WAVE string
    unsigned char fmt_chunk_marker[4];          // fmt string with trailing null
	                                            // char
    unsigned int length_of_fmt;                 // length of the format data
    unsigned int format_type;                   // format type. 1-PCM, 
	                                            // 3- IEEE float, 6 - 8bit A law
												// , 7 - 8bit mu law
    unsigned int channels;                      // no.of channels
    unsigned int sample_rate;                   // sampling rate (blocks per 
	                                            // second)
    unsigned int byterate;                      // SampleRate * NumChannels
	                                            //  * BitsPerSample/8
    unsigned int block_align;                   // NumChannels * BitsPerSample/8
    unsigned int bits_per_sample;               // bits per sample, 8- 8bits, 
	                                            // 16- 16 bits etc
    unsigned char data_chunk_header [4];        // DATA string or FLLR string
    unsigned int data_size;                     // NumSamples * NumChannels *
	                                            // * BitsPerSample/8
												// - size of the next chunk that
												// will be read
};

double* signalArray(char *file);

double* pre_emphasis(double *signal);

long frameNbr();

long frameSampleNbr();

double** hannWindow(double* PA_signal);

cplx** DFT(double** frames);

double** PEPS(cplx **DFTed_frames);

double FtoM (double f);

double MtoF (double m);

double** filterbank (double sampleRate);

double coeff(double *A, double *B, long size);

double** filterbank_energies(double **filterbank, double **power_spectrum);

double** logged_filterbank_energies(double **filterbank_energies);

double** MFCC(char *file);
#endif
