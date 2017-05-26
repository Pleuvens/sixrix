#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <portaudio.h>
#include <sndfile.h>
#include <stdint.h>

#define FRAMES_PER_BUFFER 1024

typedef struct
{
    uint16_t formatType;
    uint8_t numberOfChannels;
    uint32_t sampleRate;
    size_t size;
    float *recordedSamples;
} AudioData;

typedef struct
{
    float *snippet;
    size_t size;
} AudioSnippet;

AudioData initAudioData(uint32_t sampleRate, uint16_t channels, int type)
{
    AudioData data;
    data.formatType = type;
    data.numberOfChannels = channels;
    data.sampleRate = sampleRate;
    data.size = 0;
    data.recordedSamples = NULL;
    return data;
}


int setup_stream(PaStream *stream) {
    PaError err = paNoError;
    err = Pa_Initialize();
    if (err) {
        return err;
    }
    const PaDeviceInfo *info = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
    AudioData data = initAudioData(44100, info->maxInputChannels, paFloat32);
    AudioSnippet sampleBlock =
    {
        .snippet = NULL,
        .size = FRAMES_PER_BUFFER * sizeof(float) * data.numberOfChannels
    };

    sampleBlock.snippet = malloc(sampleBlock.size);
    PaStreamParameters inputParameters =
    {
        .device = Pa_GetDefaultInputDevice(),
        .channelCount = data.numberOfChannels,
        .sampleFormat = data.formatType,
        .suggestedLatency = info->defaultHighInputLatency,
        .hostApiSpecificStreamInfo = NULL
    };

    err = Pa_OpenStream(&stream, &inputParameters, NULL, data.sampleRate, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err) {
        return err;
    }
    Pa_StartStream(stream);
    return 1;
}

int process_stream(PaStream *stream){
  (void)stream;
  return 1;
}

void free_stream(PaStream *stream){
    Pa_CloseStream(stream);
    Pa_Terminate();
}


int main(void)
{
  //  time_t talking = 0;
    //time_t silence = 0;

    PaStream *stream = NULL;
    PaError err = setup_stream(stream);
    while (!err) {
        err = process_stream(stream);
    }
    free_stream(stream);
    return err;
}
