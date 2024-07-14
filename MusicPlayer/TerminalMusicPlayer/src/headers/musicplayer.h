
#include <signal.h> 
#include <stdbool.h> 

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
void selectPlaylist(); 

//playback.c 
void togglePause(); 
void skipSong(); 
void previousSong(); 
void toggleRepeat(); 

//threads.c
void runThreads(); 

//players/player_state.c
extern sig_atomic_t paused;
extern sig_atomic_t skip;
extern sig_atomic_t playPrevious; 
extern sig_atomic_t repeat; 




