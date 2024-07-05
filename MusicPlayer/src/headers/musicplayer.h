
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 


#include <ao/ao.h>


//wav_player.c
void playWavFile(const char *); 
//mp3_player.c
void playMp3File(const char*); 

//song_playback.c
typedef enum {
    WAV, 
    MP3, 
    INVALID, 
} AudioType; 

AudioType getAudioType(const char*);  

//music_ds.c
typedef struct SongNode { 
    char* songPath; 
    struct SongNode* next; 
    struct SongNode* prev; 
} SongNode; 

SongNode* createLinkedList(char*); 
void freeLinkedList(); 

