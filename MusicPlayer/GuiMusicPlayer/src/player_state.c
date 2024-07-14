

#include "headers/musicplayer.h"

pthread_t playerThread; 

int paused = true;

int skip = false; 

int playPrevious = false; 

int repeat = false; 

int playing = false; 

SongInfo currentSong = {NULL, NULL, NULL, NULL}; 





