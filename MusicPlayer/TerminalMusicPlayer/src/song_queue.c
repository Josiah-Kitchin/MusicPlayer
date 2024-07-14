
#include <dirent.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 

#include "headers/musicplayer.h"

#define MAX_PLAYLISTS 1000


SongQueue* createSongQueue(const char* playlistDir) { 
    //Creates a doubly linked list of song nodes within a directory
    //returns a SongQueue (which stores the start and the end) 
    //Enter NULL to search all audio files in the Music folder (not sub directories) 
    //If no valid audio files are found, returns null 

    char* playlistPath = appendPathToPlaylist(playlistDir); 

    DIR* d; 
    struct dirent* dir; 
    
    d = opendir(playlistPath); 
    if (!d) { 
	fprintf(stderr, "Failed to open %s\n", playlistPath); 
	exit(EXIT_FAILURE); 
    }

    //Creates the head. the head will not store a song or a previous node at creation. 
    SongQueue* songQueue = malloc(sizeof(SongQueue)); 
    if (!songQueue) { 
	fprintf(stderr, "Could not allocate memory for playlist head\n"); 
	exit(EXIT_FAILURE); 
    }

    songQueue->start = NULL; 
    songQueue->end = NULL; 
    SongNode* currentNode = songQueue->start; 

    //Loop through the files in the playlist directory
    while ((dir = readdir(d)) != NULL) { 
	char* audioFile = dir->d_name; 
	
	if (getAudioType(audioFile) == INVALID) { 
	    continue; 
	}

	SongNode* newNode = malloc(sizeof(SongNode)); 

	if (!newNode) { 
	    fprintf(stderr, "Failed to allocate memory for new song node\n"); 
	    exit(EXIT_FAILURE); 
	}

	char* audioFilePath = appendPathToAudioFile(playlistPath, audioFile); 

	newNode->songPath = audioFilePath; 

	if (currentNode == NULL) { 
	    newNode->prev = NULL; 
	    songQueue->start = newNode; 
	    currentNode = newNode; 
	    continue; 
	}

	newNode->prev = currentNode; 
	currentNode->next = newNode; 
	currentNode = currentNode->next; 
    } 

    closedir(d); 
    free(playlistPath); //The playlist path is strdupped so it must be freed; 

    currentNode->next = NULL; // end of the queue 
    songQueue->end = currentNode; 

    if (songQueue->start == NULL) { //Case if no valid audio files  
	free(songQueue); 
	return NULL; 
    }

    return songQueue; 
}

void startSongQueue(const SongQueue* songQueue) { 
    //Plays all songs in the playlist starting at the head. 
    if (skip) { 
	skip = !skip;  
    }
    if (!songQueue) { 
	return;
    }
    SongNode* currentNode = songQueue->start;  
    while (currentNode != NULL) { 
	printf("Playing %s\n", currentNode->songPath); 
	playAudioFile(currentNode->songPath); 

	if (playPrevious) { 
	    if (currentNode->prev != NULL) { //Will restart the song if at the start of the playlist 
		currentNode = currentNode->prev; 
	    }  
	    playPrevious = !playPrevious;
	} else { 
	    currentNode = currentNode->next;
	}
	//Repeat handling: 
	if (currentNode && currentNode->next == NULL && repeat) { 
	    currentNode = songQueue->start; // restarts at the node after the head 
	}
    }
}

void selectPlaylist() { 
    //terminal interaction for the music player 
    DIR* d; 
    struct dirent* dir; 

    char* musicPath = appendPathToPlaylist(NULL); 
    d = opendir(musicPath); 
    if (!d) { 
	fprintf(stderr, "Failed to open %s\n", musicPath); 
	exit(EXIT_FAILURE); 
    }
    char* playlists[MAX_PLAYLISTS]; 
    int curPlaylist = 1; 
    while ((dir = readdir(d)) != NULL) { 
	if (isValidDir(dir->d_name)) { 
	    playlists[curPlaylist] = dir->d_name; 
	    curPlaylist++; 
	}
    }

    while (1) { 
	if (curPlaylist < 2) { 
	    printf("No playlists found\n"); 
	    break; 
	}
	for (int i = 1; i < curPlaylist; i++) { 
	    printf("%d: %s\n", i, playlists[i]); 
	}
	int chosenDir;
	printf("Select a playlist (press 0 to exit)\n"); 
	scanf(" %d", &chosenDir); 
	if (chosenDir == 0) { 
	    break; 
	}
	runThreads(playlists[chosenDir]); 
    }

    closedir(d); 
    free(musicPath); 
}


void freeSongQueue(SongQueue* songQueue) { 
    if (!songQueue) { 
	return; 
    }
    SongNode* tmp; 
    SongNode* currentNode = songQueue->start; 

    while (currentNode != NULL) { 
	tmp = currentNode->next; 
	free(currentNode->songPath); 
	free(currentNode); 
	currentNode = tmp; 
    }
    free(songQueue); 
}






