#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include "src/headers/musicplayer.h"
#include <dirent.h> 
#include <unistd.h> 


GObject* currentSongLabel; //made global so the current song label can be changed in the song queue thread 
GObject* repeatButton; //made global so the repeat callback can change the button label 

int main (int argc, char** argv) { 
    GError *error = NULL;

    gtk_init(&argc, &argv);

    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder* builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "src/gui/builder.ui", &error) == 0)
    {
	g_printerr ("Error loading file: %s\n", error->message);
	g_clear_error (&error);
	exit(EXIT_FAILURE); 
    }

    /* Connect signal handlers to the constructed widgets. */
    GObject* window = gtk_builder_get_object (builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (quit), NULL);

    GObject* button = gtk_builder_get_object(builder, "pauseButton"); 
    g_signal_connect(button, "clicked", G_CALLBACK(pauseCallback), NULL); 

    button = gtk_builder_get_object(builder, "skipButton"); 
    g_signal_connect(button, "clicked", G_CALLBACK(skipCallback), NULL); 

    button = gtk_builder_get_object(builder, "prevButton"); 
    g_signal_connect(button, "clicked", G_CALLBACK(previousCallback), NULL); 

    repeatButton = gtk_builder_get_object(builder, "repeatButton"); 
    g_signal_connect(repeatButton, "clicked", G_CALLBACK(repeatCallback), NULL); 


    currentSongLabel = gtk_builder_get_object(builder, "currentSong"); 

    GObject* chooser_button = gtk_builder_get_object(builder, "playlistSelect");
    gtk_file_chooser_set_action(GTK_FILE_CHOOSER(chooser_button), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);

    g_signal_connect(chooser_button, "file-set", G_CALLBACK(onPlaylistSet), NULL);

    gtk_main ();

return 0;
}

