// Andrew Greensted - Feb 2010
// http://www.labbookpages.co.uk
// Version 1

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <sndfile.h>

int main(int argc, char *argv[])
{
	printf("Wav Write Test\n");
	if (argc != 2) {
		fprintf(stderr, "Expecting wav file as argument\n");
		return 1;
	}

	double freq = 440;			// Hz
	double duration = 30;		// Seconds
	int sampleRate = 44100;		// Frames / second

	// Calculate number of frames
	long numFrames = duration * sampleRate;

	// Allocate storage for frames
	double *buffer = (double *) malloc(numFrames * sizeof(double));
	if (buffer == NULL) {
		fprintf(stderr, "Could not allocate buffer for output\n");
	}

	// Create sample, a single tone
	long f;
	for (f=0 ; f<numFrames ; f++) {
		double time = f * duration / numFrames;
		buffer[f] = sin(2.0 * M_PI * time * freq);
	}

	// Set file settings, 16bit Mono PCM
	SF_INFO info;
	info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	info.channels = 1;
	info.samplerate = sampleRate;

	// Open sound file for writing
	SNDFILE *sndFile = sf_open(argv[1], SFM_WRITE, &info);
	if (sndFile == NULL) {
		fprintf(stderr, "Error opening sound file '%s': %s\n", argv[1], sf_strerror(sndFile));
		free(buffer);
		return -1;
	}

	// Write frames
	long writtenFrames = sf_writef_double(sndFile, buffer, numFrames);

	// Check correct number of frames saved
	if (writtenFrames != numFrames) {
		fprintf(stderr, "Did not write enough frames for source\n");
		sf_close(sndFile);
		free(buffer);
		return -1;
	}

	// Tidy up
	sf_write_sync(sndFile);
	sf_close(sndFile);
	free(buffer);

	return 0;
}
