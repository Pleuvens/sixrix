# include "mfcc.h"

// Décomposer le signal en plusieurs frames

char* filePath(char *file) {
	char *fileP = (char*) malloc(sizeof(char) * 1024);
	char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) == NULL)
		err(3, "fail on getcwd");
	strcpy(fileP, cwd);
	strcat(fileP, "/");
	strcat(fileP, file);
	return fileP;
}

struct headerFiller(char *file) {
	char *fileP = filePath(file)
	FILE *ptr = fopen(fileP, "rb");
	if (ptr == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

	int read = 0;

	// read header parts

	// RIFF (4 bytes)
	read = fread(header.riff, sizeof(header.riff), 1, ptr);

	// Overall size (4 bytes)
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
      // convert little endian to big endian 4 byte int
	header.overall_size  = buffer4[0] | (buffer4[1]<<8) 
									  | (buffer4[2]<<16)
									  | (buffer4[3]<<24);
	// Wave (4 bytes)
	read = fread(header.wave, sizeof(header.wave), 1, ptr);

	// FMT chunk marker (4 bytes)
	read = fread(header.fmt_chunk_marker, 
				sizeof(header.fmt_chunk_marker), 1, ptr);

	// Length of fmt (4 bytes)
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	header.length_of_fmt = buffer4[0] | (buffer4[1] << 8)
									  | (buffer4[2] << 16) 
									  | (buffer4[3] << 24);
	
	// Format type (2 bytes)
	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	header.format_type = buffer2[0] | (buffer2[1] << 8);
	char format_name[10] = "";
	if (header.format_type == 1)
        strcpy(format_name,"PCM");
    else if (header.format_type == 6)
        strcpy(format_name, "A-law");
    else if (header.format_type == 7)
        strcpy(format_name, "Mu-law");
	
	// Channels (2 bytes)
	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	header.channels = buffer2[0] | (buffer2[1] << 8);

	// Sample rate (4 bytes)
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	header.sample_rate = buffer4[0] | (buffer4[1] << 8) 
									| (buffer4[2] << 16) 
									| (buffer4[3] << 24);
	
	// Byterate (4 bytes)
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	header.byterate  = buffer4[0] | (buffer4[1] << 8) 
								  | (buffer4[2] << 16) 
								  | (buffer4[3] << 24);
	
	// Block align (2 bytes)
	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	header.block_align = buffer2[0] | (buffer2[1] << 8);

	// Bits per sample (2 bytes)
	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	header.bits_per_sample = buffer2[0] | (buffer2[1] << 8);

	// Data chunk header (4 bytes)
	read = fread(header.data_chunk_header,
				sizeof(header.data_chunk_header), 1, ptr);

	// Data size (4 bytes)
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	header.data_size = buffer4[0] | (buffer4[1] << 8) 
	 							  | (buffer4[2] << 16) 
								  | (buffer4[3] << 24 );
	
	return header;
}

double signalArray() {
	struct header = headerFiller(char *file);

	// Number of samples                                                   
    long num_samples = (8 * header.data_size) 
						/ (header.channels * header.bits_per_sample);
	// Size of each sample
	long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;

	// File duration
	float duration_in_seconds = (float) header.overall_size / header.byterate;

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

