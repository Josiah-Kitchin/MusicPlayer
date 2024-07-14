
#include <gtk/gtk.h> 
#include <stdlib.h> 
#include "headers/musicplayer.h"

void quit(GtkWindow *window) {
    gtk_main_quit(); // Quit the GTK main loop
}


void onPlaylistSet(GtkFileChooserButton *button, gpointer user_data) {
    const gchar *selected_folder = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    runQueueThread((char*)selected_folder); 
}


void updateCurrentSongLabel(const char* currentSongPath) { 
    //Updates the song path and the song info global labels 
    
    gtk_label_set_text(GTK_LABEL(titleLabel), currentSong.title); 
    gtk_label_set_text(GTK_LABEL(artistLabel), currentSong.artist); 
    gtk_label_set_text(GTK_LABEL(albumLabel), currentSong.album); 
    gtk_label_set_text(GTK_LABEL(yearLabel), currentSong.year); 

    char* currentSongFile = parseFileFromPath(currentSongPath); 
    char fileIntro[] = "Now playing: "; 
    char displayMessage[strlen(fileIntro) + strlen(currentSongFile) + 1]; 
    strcpy(displayMessage, fileIntro); 
    strcat(displayMessage, currentSongFile); 
    gtk_label_set_text(GTK_LABEL(currentSongLabel), displayMessage); 
    free(currentSongFile); 
}

void removeCurrentSongLabel() { 
    gtk_label_set_text(GTK_LABEL(titleLabel), "Unknown Title"); 
    gtk_label_set_text(GTK_LABEL(artistLabel), "Unknown Artist"); 
    gtk_label_set_text(GTK_LABEL(albumLabel), "Unknown Album"); 
    gtk_label_set_text(GTK_LABEL(yearLabel), "Unknown Year"); 

    gtk_label_set_text(GTK_LABEL(currentSongLabel), "Now Playing: None"); 
}

