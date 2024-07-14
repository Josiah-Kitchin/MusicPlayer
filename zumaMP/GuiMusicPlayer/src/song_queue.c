
#include <dirent.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <gtk/gtk.h> 

#include "headers/musicplayer.h"

//#define LOG 
#define MAX_PLAYLISTS 1000


//Songs are stored in doubly linked lists

SongQueue* createSongQueue(const char* playlistDir) { 
    //Creates a doubly linked list of song nodes within a directory
    //returns a SongQueue (which stores the start and the end) 
    //Enter NULL to search all audio files in the Music folder (not sub directories) 
    //If no valid audio files are found, returns null 

    DIR* d; 
    struct dirent* dir; 
    
    d = opendir(playlistDir); 
    if (!d) { 
	fprintf(stderr, "Failed to open %s\n", playlistDir); 
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

	char* audioFilePath = appendPathToAudioFile(playlistDir, audioFile); 

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

    if (songQueue->start == NULL) { //Case if no valid audio files  
	free(songQueue); 
	return NULL; 
    }

    currentNode->next = NULL; // end of the queue 
    songQueue->end = currentNode; 


    return songQueue; 
}

void startSongQueue(const SongQueue* songQueue) { 
    //Plays all songs in the playlist starting at the head. 
    playing = true;  //Used to manage threads (in playback.c)
    SongNode* currentNode = songQueue->start;  

    
    while (currentNode != NULL) { 
	//Repeat handling: 

	#ifdef LOG
	    printf("Playing %s\n", currentNode->songPath); 
	#endif

	updateCurrentSongLabel(currentNode->songPath); 

	playAudioFile(currentNode->songPath); 
	
	if (!playing) { //if the user changes the playlist  
	    break; 
	}

	if (playPrevious) { 
	    if (currentNode->prev != NULL) { //Will restart the song if at the start of the playlist 
		currentNode = currentNode->prev; 
	    }  
	    playPrevious = !playPrevious;
	} else { 
	    currentNode = currentNode->next;
	}

	if (currentNode && currentNode->next == NULL && repeat) { 
	    currentNode = songQueue->start; // restarts at the node after the head 
	}
    }
    removeCurrentSongLabel(); 
    playing = false; 
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






