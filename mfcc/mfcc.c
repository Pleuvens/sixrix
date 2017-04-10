# include "mfcc.h"
  
struct HEADER header;

long num_samples;

// Renvoie le tableau du signal
double* signalArray(char *file) {
	// File Path

	char *fileP = (char*) malloc(sizeof(char) * 1024);
	char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) == NULL)
		err(3, "fail on getcwd");
	strcpy(fileP, cwd);
	strcat(fileP, "/");
	strcat(fileP, file);

	FILE *ptr = fopen(fileP, "rb");
	if (ptr == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

	// Read Header Parts

	unsigned char buffer4[4];
	unsigned char buffer2[2];

	// RIFF (4 bytes)
	fread(header.riff, sizeof(header.riff), 1, ptr);

	// Overall size (4 bytes)
	fread(buffer4, sizeof(buffer4), 1, ptr);
	 // convert little endian to big endian 4 byte int
	header.overall_size  = buffer4[0] | (buffer4[1]<<8) 
		| (buffer4[2]<<16)
		| (buffer4[3]<<24);
	// Wave (4 bytes)
	fread(header.wave, sizeof(header.wave), 1, ptr);

	// FMT chunk marker (4 bytes)
	fread(header.fmt_chunk_marker, 
			sizeof(header.fmt_chunk_marker), 1, ptr);

	// Length of fmt (4 bytes)
	fread(buffer4, sizeof(buffer4), 1, ptr);
	header.length_of_fmt = buffer4[0] | (buffer4[1] << 8)
		| (buffer4[2] << 16) 
		| (buffer4[3] << 24);

	// Format type (2 bytes)
	fread(buffer2, sizeof(buffer2), 1, ptr);
	header.format_type = buffer2[0] | (buffer2[1] << 8);
	char format_name[10] = "";
	if (header.format_type == 1)
		strcpy(format_name,"PCM");
	else if (header.format_type == 6)
		strcpy(format_name, "A-law");
	else if (header.format_type == 7)
		strcpy(format_name, "Mu-law");

	// Channels (2 bytes)
	fread(buffer2, sizeof(buffer2), 1, ptr);
	header.channels = buffer2[0] | (buffer2[1] << 8);

	// Sample rate (4 bytes)
	fread(buffer4, sizeof(buffer4), 1, ptr);
	header.sample_rate = buffer4[0] | (buffer4[1] << 8) 
		| (buffer4[2] << 16) 
		| (buffer4[3] << 24);

	// Byterate (4 bytes)
	fread(buffer4, sizeof(buffer4), 1, ptr);
	header.byterate  = buffer4[0] | (buffer4[1] << 8) 
		| (buffer4[2] << 16) 
		| (buffer4[3] << 24);

	// Block align (2 bytes)
	fread(buffer2, sizeof(buffer2), 1, ptr);
	header.block_align = buffer2[0] | (buffer2[1] << 8);

	// Bits per sample (2 bytes)
	fread(buffer2, sizeof(buffer2), 1, ptr);
	header.bits_per_sample = buffer2[0] | (buffer2[1] << 8);

	// Data chunk header (4 bytes)
	fread(header.data_chunk_header,
			sizeof(header.data_chunk_header), 1, ptr);

	// Data size (4 bytes)
	fread(buffer4, sizeof(buffer4), 1, ptr);
	header.data_size = buffer4[0] | (buffer4[1] << 8) 
		| (buffer4[2] << 16) 
		| (buffer4[3] << 24 );


	// Number of samples                                                   
	num_samples = (8 * header.data_size) 
		/ (header.channels * header.bits_per_sample);

	// Size of each sample
	long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;

	// File duration
	// float duration_in_seconds = (float) header.overall_size / header.byterate;

	double *tab = malloc(sizeof(double) * num_samples);
	long i = 0;
	char data_buffer[size_of_each_sample];
	int  size_is_correct = true;
	long bytes_in_each_channel = (size_of_each_sample / header.channels);
	if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
		size_is_correct = false;
	}
	if (size_is_correct) {
		for (i =1; i <= num_samples; i++) {
			int read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
			if (read == 1) {
				unsigned int  xchannels = 0;
				int data_in_channel = 0;
				for (xchannels = 0; xchannels < header.channels; xchannels++ ) {
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
					tab[i] = data_in_channel;
				}
			}
		}
	}
	return tab;
}

// Pré-accentue le signal
double* pre_emphasis(double *signal) {
	double *PE_signal = malloc(sizeof(double) * num_samples);
	PE_signal[0] = signal[0];
	for (int i = 1; i < num_samples; i++) {
		PE_signal[i] = signal[i] - (0.95 * signal[i-1]);
	}
	return PE_signal;
}

long frameNbr() {
	return num_samples / (floor(header.sample_rate * 0.020)); // = 20 ms
													//(standard frame length)
}

long frameSampleNbr() {
	return floor(header.sample_rate * 0.020);
}

//Applique hanning sur les frames
double** hannWindow(double* PA_signal) {
	long frameSampleNbr = frameSampleNbr();
	long step = floor(frameSampleNbr/2);
	double **frames = malloc(sizeof(double*) * frameNbr());
	long i = 0; // scanning index for PA_signal
	long j = 0; // index for frames
	long k = 0; // index for frames[j] 
	while (i < num_samples) {
		frames[j] = malloc(sizeof(double) * frameSampleNbr);
		for (k = 0; k < frameSampleNbr; k++, i++) {
			frames[j][k] = PA_signal[i] * 
						(0.54 - 0.46 * cos((2 * PI * i) / (frameSampleNbr-1)));
		}
		j++;
		i -= step;
	}
	return frames;
}

//DFT on each frames
cplx** DFT(double** frames) {
	long frameNbr = frameNbr();
	long frameSampleNbr = frameSampleNbr();
	long k = 1;
    while ((k*2) < num_samples){
		k *= 2;
	}
	cplx DFTframes = malloc(sizeof(cplx*) * frameNbr);
	for (long i = 0; i < frameNbr; i++) {
		DFTframes[i] = malloc(sizeof(cplx) * frameSampleNbr);
		fft(DFTframes[i], k, PI);
	}
	return DFTframes;
}

//Periodogram estimate of the power spectrum
cplx** PEPS(cplx **DFTed_frames) {
	long frameNbr = frameNbr();
	long frameSampleNbr = frameSampleNbr();
	for (long i = 0; i < frameNbr; i++) {
		for (long j = 0; j < frameSampleNbr; j++) {
			cplx Z = DFTed_frames[i][j];
			DFTed_frames[i][j] = (1/frameSampleNbr) * 
					((creal (Z) * creal(Z)) + (cimag (Z) * cimag (Z)));
		}
	}
	return DFTed_frames;
}

double FtoM (double f) {
	return 1127 * log(1 + (f / 700)); // base-e log (not 10)
}

double MtoF (double m) {
	return 700 * (exp(m / 1127) - 1);
}

double** filterbank (double sampleRate, double FFTsize) {
	double filterbanksNbr = 26
	double lower_f = 20;
	double upper_f = 7600;
	double lower_mel = FtoM(lower_f);
	double upper_mel = FtoM(upper_f);
	double step = (upper_mel - lower_mel) / (filterbanksNbr + 1);

	double *points = malloc(sizeof(double) * (filterbanksNbr + 2));
	points[0] = lower_mel;
	points[filterbanksNbr + 1] = upper_mel;
	for (long i = 1; i < (filterbanksNbr + 1); i++) {
		points[i] = lower_mel + step;
		step *= 2;
	} // *points is a pointer on mel number array

	for (long i = 0; i < (filterbanksNbr + 1); i++) {
		points[i] = MtoF(points[i]);
	} // *points is now a pointer on frequency array

	for (long i = 0; i < (filterbanksNbr + 1); i++) {
		points[i] = floor((FFTsize + 1) * points[i] / sampleRate);
	}

	double **filterbanks = malloc(sizeof(double*) * filterbanksNbr);
	for (long i = 0; i < filterbanksNbr; i++) {
		filterbanks[i] = malloc(sizeof(double) * ((FFTsize / 2) + 1));
		for (long j = 0; j < ((FFTsize / 2) + 1); j++) {
			if (j < points[i])
				filterbanks[i][j] = 0;
			if (points[i] <= j && j <= points[i + 1])
				filterbanks[i][j] = 
								(j - points[i]) / (points[i + 1] - points[i]);
			if (points[i + 1] <= j && j <= points[i + 2])
				filterbanks[i][j] = 
						(points[i + 2] - j) / (points[i + 2] - points[i + 1]);
			if (points[i + 2] < j)
				filterbanks[i][j] = 0;
		}
	}
	return filterbanks;
}

double* filterbank_energies(double **filterbank, double filterbankNbr,
							cplx **power_spectrum, double FFTsize) {
	double *energies = malloc(sizeof(double) * filterbankNbr);
	for (long i = 0; i < filterbankNbr; i++) {
		double sum = 0;
		for (long j = 0; j < ((FFTsize / 2) + 1); j++) {
			sum += filterbank[i][j] * power_spectrum[i]
		}
	}
}

int main(int argc, char **argv) {
	(void)argc;
	double *tab = signalArray(argv[1]);
	printf("Normal signal :\n");
	for (int i = 0; i < 100; i++) {
		printf("%f   #    ", tab[i]);
	}
	printf("\n\nPre-amplified signal :\n");
	double *PE_tab = pre_emphasis(tab);
	for (int i = 0; i < 100; i++) {
		printf("%f   #    ", PE_tab[i]);
	}
	printf("\n");
	return 0;
}
