
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <dirent.h> 

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

    //if (strcmp(file + (len-4), ".wav") == 0) {
	//return WAV; 
    //}
    return INVALID; 
}

void playAudioFile(const char* audioFilePath) { 
    //Plays the correct function for an audiofile, and updates the gui labels 
    switch (getAudioType(audioFilePath)) { 
	case MP3: 
	    fillMp3SongInfo(audioFilePath); 
	    updateCurrentSongLabel(audioFilePath); 
	    playMp3File(audioFilePath); 
	    break; 
	//case WAV: 
	    //playWavFile(audioFilePath); 
	    //break; 
    }
}


char* appendPathToAudioFile(const char* playlistPath, const char* audioFile) { 
    char audioFilePath[strlen(playlistPath) + strlen(audioFile) + 2]; 
    strcpy(audioFilePath, playlistPath); 
    strcat(audioFilePath, "/"); 
    strcat(audioFilePath, audioFile); 
    return strdup(audioFilePath); 
}

char* parseFileFromPath(const char* path) { 
    int finalSlashIdx = 0; 
    for (int i = 0; i < strlen(path); i++) { 
	if (path[i] == '/') { 
	    finalSlashIdx = i; 
	}
    }
    return strdup(path + finalSlashIdx + 1); 
}





