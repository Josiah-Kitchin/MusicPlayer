
#include <dirent.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 

#include "headers/musicplayer.h"


//Songs are stored in doubly linked lists
//A cycle is created if repeat is on. Otherwise ends in null. 

SongNode* createLinkedList(char* playlistDir) { 
    //Creates a doubly linked list of song nodes within a directory
    //Returns the head (with song and prev being null) 
    //Enter NULL to search all in the Music folder 
    //If no valid audio files are found, returns null 

    //Append the music path to the playlist 
    char* homeDir = getenv("HOME"); 
    char musicDir[] = "/Music"; 
    char playlistPath[strlen(homeDir) + strlen(musicDir) + strlen(playlistDir) + 1]; 
    strcpy(playlistPath, homeDir); 
    strcat(playlistPath, musicDir); 
    strcat(playlistPath, playlistDir); 

    DIR* d; 
    struct dirent* dir; 
    
    d = opendir(playlistPath); 
    if (!d) { 
	fprintf(stderr, "Failed to open Music directory\n"); 
	exit(EXIT_FAILURE); 
    }

    SongNode* head = malloc(sizeof(SongNode)); 
    head->next = NULL; 
    head->prev = NULL; 
    head->songPath = NULL; 
    SongNode* currentNode = head; 

    //Loop through the files in the playst directory
    while ((dir = readdir(d)) != NULL) { 
	char* audioFile = dir->d_name; 
	
	if (getAudioType(audioFile) == INVALID) { 
	    continue; 
	}

	currentNode->next = malloc(sizeof(SongNode)); 

	//Append the full path to the audio file 
	char audioFilePath[strlen(playlistPath) + strlen(audioFile) + 2]; 
	strcpy(audioFilePath, playlistPath); 
	strcat(audioFilePath, "/"); 
	strcat(audioFilePath, audioFile); 

	currentNode->next->songPath = audioFilePath; 
	currentNode->next->prev = currentNode; 
	currentNode = currentNode->next; 
    } 
    closedir(d); 
    currentNode->next = NULL; 

    if (head->next == NULL) { //Case if no valid audio files  
	free(head); 
	return NULL; 
    }

    return head; 
}


void freeLinkedList(SongNode* head) { 
    SongNode* tmp; 
    SongNode* currentNode = head; 

    while (head != NULL) { 
	tmp = currentNode->next; 
	free(currentNode); 
	currentNode = tmp; 
    }
}






