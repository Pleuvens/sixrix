# include "mfcc.h"

struct HEADER header;

long num_samples;

double sampleRate;

long FFTsize = 512;

long frameNbr_;

long filterbankNbr;

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

  sampleRate = header.sample_rate;

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
  long frameSampleNbr_ = frameSampleNbr();
  long step = floor(frameSampleNbr_ / 2);
  double **frames = malloc(sizeof(double*) * frameNbr());
  long i = 0; // scanning index for PA_signal
  long j = 0; // index for frames
  long k = 0; // index for frames[j] 
  while (i < num_samples) {
    frames[j] = malloc(sizeof(double) * frameSampleNbr_);
    for (k = 0; k < frameSampleNbr_; k++, i++) {
      frames[j][k] = PA_signal[i] * 
	(0.54 - 0.46 * cos((2 * PI * i) / (frameSampleNbr_ - 1)));
    }
    j++;
    i -= step;
  }
  return frames;
}

/*cplx DFT_i(double *hannWindow_i) {
	cplx DFT_i = malloc(sizeof(cplx) * FFTsize);
	long frameSampleNbr_ = frameSampleNbr();
	for (long j = 0; j < FFTsize; j++) {
		cplx sum;
		for (long n = 0; n < frameSampleNbr_; n++) {
			sum += hannWindow_i[n] * exp()
		}
	}
} */

//DFT on each frames
cplx** DFT(double** frames) {
	long frameNbr_ = frameNbr();
	long frameSampleNbr_ = frameSampleNbr();
	/*
	long k = 1;
	while ((k*2) < num_samples){
		k *= 2;
	}
	*/

	long k = 1;
	while ((k * 2) < frameSampleNbr_ ) {
		k *= 2;
	}
	k *= 2;

	cplx **DFTframes = malloc(sizeof(cplx*) * frameNbr_);
	for (long i = 0; i < frameNbr_; i++) {
		DFTframes[i] = malloc(sizeof(cplx) * k);
		for (long j = 0; j < frameSampleNbr_; j++) {
			DFTframes[i][j] = frames[i][j];
		}
		for (long j = frameSampleNbr_; j < k; j++) {
			DFTframes[i][j] = 0;
		}
		fft(DFTframes[i], k, PI);
		cplx DFT_[FFTsize];
		for (long l = 0; l < FFTsize; l++) {
			DFT_[l] = DFTframes[i][l];
		}
		DFTframes[i] = DFT_;
	}
	return DFTframes;
}

//Periodogram estimate of the power spectrum
double** PEPS(cplx **DFTed_frames) {
	long frameNbr_ = frameNbr();
	double** power_spec = malloc(sizeof(double*) * frameNbr_);
	for (long i = 0; i < frameNbr_; i++) {
		power_spec[i] = malloc(sizeof(double) * FFTsize); 
		for (long j = 0; j < FFTsize; j++) {
			cplx Z = DFTed_frames[i][j];
			power_spec[i][j] = (1 / FFTsize) * 
				((creal (Z) * creal(Z)) + (cimag (Z) * cimag (Z)));
		}
	}
	return power_spec;
}

double FtoM (double f) {
	return 1127 * log(1 + (f / 700)); // base-e log (not 10)
}

double MtoF (double m) {
	return 700 * (exp(m / 1127) - 1);
}

double** filterbank (double sampleRate, double FFTsize) {
	long filterbanksNbr = 26;
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

double coeff(double *A, double *B, long size) {
	double coeff = 0;
	for (long i = 0; i < size; i++) {
		coeff += A[i] * B[i];
	}
	return coeff;
}

double** filterbank_energies(double **filterbank,
		long filterbanksNbr,
		double **power_spectrum,
		double FFTsize,
		long frameNbr_) {

	double **energies = malloc(sizeof(double*) * frameNbr_);
	for (long i = 0; i < frameNbr_; i++) {
		energies[i] = malloc(sizeof(double) * filterbanksNbr);
		for (long j = 0; j < filterbanksNbr; j++) {
			energies[i][j] = coeff(power_spectrum[i], filterbank[j], 
					(FFTsize / 2) + 1); 
		}
	}
	return energies;
}

double** logged_filterbank_energies(double **filterbank_nrgies,
		long frameNbr_,
		long filterbankNbr) {

	for (long i = 0; i < frameNbr_; i++) {
		for (long j = 0; j < filterbankNbr; j++) {
			filterbank_nrgies[i][j] = log(filterbank_nrgies[i][j]);
		}
	}
	return filterbank_nrgies;
}

double** DCT_II(double **logged_filterbank_energies,
		long frameNbr_,
		long filterbankNbr) {

	double **DCT = malloc(sizeof(double*) * frameNbr_);
	for (long i = 0; i < frameNbr_; i++) {
		DCT[i] = malloc(sizeof(double) * filterbankNbr);
		for (long j = 0; j < filterbankNbr; j++) {
			long k = 0;
			double sum = 0;
			while (k < filterbankNbr) {
				sum += logged_filterbank_energies[i][k]
					* cos((PI / filterbankNbr) * (k + (1/2)) * j);
			}
			DCT[i][j] = sum;
		}
	}
	return DCT;
}

/*int main(int argc, char* argv[]) {
	(void)argc;
	double *signal = signalArray(argv[1]);*/

double **MFCC(char *file) {
	double *signal = signalArray(file);
	double *signal_pre_emphasis = pre_emphasis(signal);
	double **framed_signal = hannWindow(signal_pre_emphasis);
	cplx **DFTed_frames = DFT(framed_signal);
	double **power_spectrum = PEPS(DFTed_frames);
	double **filterBank = filterbank(sampleRate, FFTsize);
	double **energies = filterbank_energies(filterBank, filterbankNbr,
			power_spectrum, FFTsize, frameNbr_);
	double **logged_energies = logged_filterbank_energies(energies, frameNbr_,
			filterbankNbr);
	double **DCT_of_energies = DCT_II(logged_energies, frameNbr_,
			filterbankNbr);

	double **feat_vect = malloc(sizeof(double*) * frameNbr_);
	for (long i = 0; i < frameNbr_; i++) {
		feat_vect = malloc(sizeof(double) * 13);
		for (long j = 0; j < 13; j++) {
			feat_vect[i][j] = DCT_of_energies[i][j];
		}
	}
	return feat_vect;
	//  return 0;
}
