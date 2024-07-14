
#include <pthread.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "headers/musicplayer.h" 

#define LOG

//Concurrency. Handles signals 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int signal_to_send = 0;

static void* signalSender(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (signal_to_send == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        
        if (signal_to_send == 1) {
	    togglePause(); 
	    if (paused) { 
		printf("Paused\n"); 
	    } else { 
		printf("Unpaused\n"); 
	    }

        } else if (signal_to_send == 2) {
	    printf("Skipping Song\n"); 
	    skipSong(); 

        } else if (signal_to_send == 3) { 
	    printf("Playing Previous Song\n"); 
	    previousSong(); 
	} else if (signal_to_send == 4) { 
	    toggleRepeat(); 
	    if (repeat) { 
		printf("Repeat Enabled\n"); 
	    } else { 
		printf("Repeat Disabled\n"); 
	    }
	}
        
        signal_to_send = 0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

static void* playlistStartCallback(void* arg) { 
    SongQueue* songQueue = createSongQueue((char*)arg);   
    if (songQueue == NULL) { 
	#ifdef LOG 
	    printf("Playlist %s is empty\n", (char*)arg); 
	#endif
	return NULL; 
    }
    startSongQueue(songQueue); 
    freeSongQueue(songQueue); 
    printf("End of playlist\n"); 
    return NULL; 
}

void handleKeyboardInput() {
    
    char input; 
    
    while (1) {
        scanf(" %c", &input); 
	switch (input) { 
	    case 'p':  
		pthread_mutex_lock(&mutex);
		signal_to_send = 1;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		break;
	    case 's':  
		pthread_mutex_lock(&mutex);
		signal_to_send = 2;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		break;
	    case 'b': 
		pthread_mutex_lock(&mutex);
		signal_to_send = 3;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		break; 
	    case 'r': 
		pthread_mutex_lock(&mutex); 
		signal_to_send = 4; 
		pthread_cond_signal(&cond); 
		pthread_mutex_unlock(&mutex); 
		break; 
	    case 'q': 
		printf("Exiting Playlist\n"); 
		return; 
	    case 'h':
		printf("p: pause\ns: skip song\nb: previous song\nr: enable/disable repeat\nq: quit\n"); 
		break; 
	    default: 
		printf("Invalid input. Please try again (press h for help).\n");
		break;
        }
    }
}

void runThreads(const char* playlistPath) { 
    pthread_t sender_thread, player_thread;
    
    //Thread for taking in input signals 
    if (pthread_create(&sender_thread, NULL, signalSender, NULL) != 0) {
        perror("Error creating sender thread");
        exit(EXIT_FAILURE);
    }

    //Thread for playing songs 
    if (pthread_create(&sender_thread, NULL, playlistStartCallback, (void*)playlistPath) != 0) {
        perror("Error creating sender thread");
        exit(EXIT_FAILURE);
    }
    
    // Handles keyboard input
    
    handleKeyboardInput();
    
    // Clean up thread resources

    pthread_cancel(player_thread);
    pthread_cancel(sender_thread);
}







