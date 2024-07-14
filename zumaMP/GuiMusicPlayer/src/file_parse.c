
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

    if (strcmp(file + (len-4), ".wav") == 0) {
	return WAV; 
    }
    return INVALID; 
}

void playAudioFile(const char* audioFilePath) { 
    switch (getAudioType(audioFilePath)) { 
	case MP3: 
	    playMp3File(audioFilePath); 
	    break; 
	case WAV: 
	    playWavFile(audioFilePath); 
	    break; 
    }
}

char* appendPathToPlaylist(const char* playlistDir) {
    char* homeDir = getenv("HOME"); 
    char musicDir[] = "/Music/"; 

    int playlistDirLen; 
    if ( playlistDir == NULL) { 
	playlistDirLen = 0; 
    } else { 
	playlistDirLen = strlen(playlistDir); 
    }

    char playlistPath[strlen(homeDir) + strlen(musicDir) + playlistDirLen + 1]; 
    strcpy(playlistPath, homeDir); 
    strcat(playlistPath, musicDir); 

    if (playlistDir != NULL) { 
	strcat(playlistPath, playlistDir); 
    }
    return strdup(playlistPath); 
}


char* appendPathToAudioFile(const char* playlistPath, const char* audioFile) { 
    char audioFilePath[strlen(playlistPath) + strlen(audioFile) + 2]; 
    strcpy(audioFilePath, playlistPath); 
    strcat(audioFilePath, "/"); 
    strcat(audioFilePath, audioFile); 
    return strdup(audioFilePath); 
}

bool isValidDir(const char* dirName) { 
    //Checks if the name is valid and it is not empty
    if (!dirName) { 
	return false; 
    }
    if (strcmp(dirName, ".") == 0) { 
	return false; 
    }
    if (strcmp(dirName, "..") == 0) { 
	return false; 
    }

    DIR* d; 
    struct dirent* dir; 

    char* dirPath = appendPathToPlaylist(dirName); 
    d = opendir(dirPath); 
    if (!d) { 
	fprintf(stderr, "Failed to open %s\n", dirPath); 
	exit(EXIT_FAILURE); 
    }
    int numDir = 0; 
    while((dir = readdir(d)) != NULL) { 
	numDir++; 
    }
    closedir(d); 
    free(dirPath); 

    if (numDir < 3) { // to account for .. and .  
	return false; 
    }

    return true; 
}

char* parseFileFromPath(char* path) { 
    int finalSlashIdx = 0; 
    for (int i = 0; i < strlen(path); i++) { 
	if (path[i] == '/') { 
	    finalSlashIdx = i; 
	}
    }
    return strdup(path + finalSlashIdx + 1); 
}





