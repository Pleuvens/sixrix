#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  int i, count = SDL_GetNumAudioDevices(0);

  for (i = 0; i < count; ++i) {
    SDL_Log("Audio device %d: %s", i, SDL_GetAudioDeviceName(i, 0));
  }

  return 0;
}
