
#include <gtk/gtk.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include "headers/musicplayer.h"


void runQueueThread(const char* dir) {
    //Runs the song queue in a seperate thread to keep the gui and playback functioning 
    if (playing) { 
	changePlaylist(); 
	pthread_cancel(playerThread); 
    }

    //Thread for taking in input signals 
    if (pthread_create(&playerThread, NULL, runQueue, (void*)dir) != 0) {
        perror("Error creating player thread");
        exit(EXIT_FAILURE);
    }
}

void* runQueue(void* dir) { 
    SongQueue* sq = createSongQueue((gchar*)dir); 
    if (!sq) { 
	removeCurrentSongLabel(); 
	pthread_cancel(playerThread); 
	return NULL; 
    }
    startSongQueue(sq); 
    freeSongQueue(sq); 
    pthread_cancel(playerThread); 
    
}

void pauseCallback(GtkWindow* window, gpointer data) { 
    paused = !paused; 
    if (paused) { 
	gtk_button_set_image(GTK_BUTTON(pauseButton), 
	gtk_image_new_from_icon_name("media-playback-start-symbolic", GTK_ICON_SIZE_BUTTON));
	gtk_button_set_label(GTK_BUTTON(pauseButton), "Play"); 
    } else { 
	gtk_button_set_image(GTK_BUTTON(pauseButton), 
	gtk_image_new_from_icon_name("media-playback-pause-symbolic", GTK_ICON_SIZE_BUTTON));
	gtk_button_set_label(GTK_BUTTON(pauseButton), "Pause"); 
    }
}

void skipCallback(GtkWindow* window, gpointer data) { 
    if (playing) { 
	skip = !skip; 
    }
}

void previousCallback(GtkWindow* window, gpointer data) { 
    if (playing) { 
	skip = !skip;
	playPrevious = !playPrevious; 
    }
}

void repeatCallback(GtkWindow* window, gpointer data) { 
    repeat = !repeat; 
    if (repeat) { 
	gtk_button_set_label(GTK_BUTTON(repeatButton), "Disable Repeat"); 
    } else { 
	gtk_button_set_label(GTK_BUTTON(repeatButton), "Enable Repeat"); 
    }
}

void changePlaylist() { 
    if (!paused) { 
	pauseCallback(NULL, NULL); 
    }
    
    playing = false;   
}



