
#include <signal.h> 
#include <stdbool.h> 
#include <gtk/gtk.h> 

//wav_player.c
void playWavFile(const char*); 
//mp3_player.c
void playMp3File(const char*); 

//file_parse.c 
typedef enum {
    WAV, 
    MP3, 
    INVALID, 
} AudioType; 

AudioType getAudioType(const char*);  
void playAudioFile(const char*); 
char* appendPathToPlaylist(const char*); 
char* appendPathToAudioFile(const char*, const char*);  
bool isValidDir(const char*); 
char* parseFileFromPath(char*); 


//song_queue.c
typedef struct SongNode { 
    char* songPath; 
    struct SongNode* next; 
    struct SongNode* prev; 
} SongNode; 

typedef struct { 
    SongNode* start; 
    SongNode* end; 
} SongQueue; 

SongQueue* createSongQueue(const char*); 
void startSongQueue(const SongQueue*); 
void freeSongQueue(); 

//gui_functions.c

void quit(GtkWindow*); 
void onPlaylistSet(GtkFileChooserButton*, gpointer); 
void updateCurrentSongLabel(char*); 
void removeCurrentSongLabel(); 



//playback.c
void runQueueThread(const char*); 
void* runQueue(void*); 

void pauseCallback(GtkWindow*, gpointer); 
void skipCallback(GtkWindow*, gpointer); 
void previousCallback(GtkWindow*, gpointer); 
void repeatCallback(GtkWindow*, gpointer); 
void changePlaylist(); 

//players/player_state.c
extern int paused;
extern int skip;
extern int playPrevious; 
extern int repeat; 
extern int playing; 

extern pthread_t playerThread; 
extern GObject* currentSongLabel; 
extern GObject* repeatButton; 



