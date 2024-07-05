

#include <dirent.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 

#include "headers/musicplayer.h" 

AudioType getAudioType(const char* file) { 
    AudioType type; 
    int len = strlen(file); 
    if (len <= 4) { 
	return INVALID; 
    }

    if (strcmp(file + (len-4), ".mp3") == 0) {
	return MP3; 
    }

    if (strcmp(file + (len-4), ".wav") == 0) {
	return WAV; 
    }
    return INVALID; 
}

