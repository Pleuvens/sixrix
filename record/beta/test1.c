#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"

#include <math.h>
#include <malloc.h>
#include <sndfile.h>
#include <err.h>

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (2)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0)

#define WRITE_TO_FILE   (1)

/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#endif

typedef struct
{
    int          frameIndex;  /* Index into sample array. */
    int          maxFrameIndex;
    SAMPLE      *recordedSamples;
}
paTestData;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
     const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
     SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
     long framesToCalc;
     long i;
     int finished;
     unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

     (void) outputBuffer; /* Prevent unused variable warnings. */
     (void) timeInfo;
     (void) statusFlags;
     (void) userData;

     if( framesLeft < framesPerBuffer )
     {
         framesToCalc = framesLeft;
         finished = paComplete;
     }
     else
     {
         framesToCalc = framesPerBuffer;
         finished = paContinue;
     }

     if( inputBuffer == NULL )
     {
         for( i=0; i<framesToCalc; i++ )
         {
             *wptr++ = SAMPLE_SILENCE;  /* left */
             if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
         }
     }
     else
     {
         for( i=0; i<framesToCalc; i++ )
         {
             *wptr++ = *rptr++;  /* left */
             if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
         }
     }
     data->frameIndex += framesToCalc;
     return finished;
 }



int main(int argc, char const *argv[])
 {
    (void)argc;
     PaStreamParameters  inputParameters;//,outputParameters;
     PaStream*           stream;
     PaError             err = paNoError;
     paTestData          data;
     int                 i;
     int                 totalFrames;
     int                 numSamples;
     int                 numBytes;
     SAMPLE              max, val;
     double              average;

     printf("patest_record.c\n"); fflush(stdout);

     data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
     data.frameIndex = 0;
     numSamples = totalFrames * NUM_CHANNELS;
     numBytes = numSamples * sizeof(SAMPLE);
     data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */


     if( data.recordedSamples == NULL )
     {
         printf("Could not allocate record array.\n");
         goto done;
     }
     for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;

     err = Pa_Initialize();
     if( err != paNoError ) goto done;

     inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
     if (inputParameters.device == paNoDevice) {
         fprintf(stderr,"Error: No default input device.\n");
         errx(3,"No device found");
         goto done;
     }
     else{
     printf("Default device = %d\n",inputParameters.device );
     }

     inputParameters.channelCount = 2;                    /* stereo input */
     inputParameters.sampleFormat = PA_SAMPLE_TYPE;
     inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
     inputParameters.hostApiSpecificStreamInfo = NULL;

     /* Record some audio. -------------------------------------------- */
     err = Pa_OpenStream(
               &stream,
               &inputParameters,
               NULL,                  /* &outputParameters, */
               SAMPLE_RATE,
               FRAMES_PER_BUFFER,
               paClipOff,      /* we won't output out of range samples so don't bother clipping them */
               recordCallback,
               &data );
     if( err != paNoError ) goto done;

     err = Pa_StartStream( stream );
     if( err != paNoError ) goto done;
     printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

     float r=1;
     while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
     {
         Pa_Sleep(1000);
          r=((float)data.frameIndex/(float)data.maxFrameIndex)*100;
          printf("Listening! %d%%\n",(int)r );
     }
     if( err < 0 ) goto done;

     err = Pa_CloseStream( stream );
     if( err != paNoError ) goto done;

     /* Measure maximum peak amplitude. */
     max = 0;
     average = 0.0;
     for( i=0; i<numSamples; i++ )
     {
         val = data.recordedSamples[i];
         if( val < 0 ) val = -val; /* ABS */
         if( val > max )
         {
             max = val;
         }
         average += val;
     }

     average = average / (double)numSamples;

     printf("sample max amplitude = "PRINTF_S_FORMAT"\n", max );
     printf("sample average = %lf\n", average );


/***************************WAV CUSTOM******************************************/
     // Set file settings, 16bit Mono PCM
      // Allocate storage for frames

    long numFrames=totalFrames;

     SF_INFO info;
     info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;
     info.channels = 2;
     info.samplerate = SAMPLE_RATE;

     // Open sound file for writing
     if (argv[1]==NULL) {
       errx(3,"Error no name for the output file");
     }
     SNDFILE *sndFile = sf_open(argv[1], SFM_WRITE, &info);
     if (sndFile == NULL) {
       fprintf(stderr, "Error opening sound file '%s': %s\n", argv[1], sf_strerror(sndFile));
       return -1;
     }

     // Write frames
     long writtenFrames = sf_writef_float(sndFile, data.recordedSamples, numFrames);

     // Check correct number of frames saved
     if (writtenFrames != numFrames) {
       fprintf(stderr, "Did not write enough frames for source\n");
       sf_close(sndFile);
       return -1;
     }

     // Tidy up
     sf_write_sync(sndFile);
     sf_close(sndFile);

done:
     Pa_Terminate();
     if( data.recordedSamples )       /* Sure it is NULL or valid. */
         free( data.recordedSamples );
     if( err != paNoError )
     {
         fprintf( stderr, "An error occured while using the portaudio stream\n" );
         fprintf( stderr, "Error number: %d\n", err );
         fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
         err = 1;          /* Always return 0 or 1, but no other return codes. */
     }
     return err;
 }
