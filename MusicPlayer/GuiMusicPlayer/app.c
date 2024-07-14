#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include "src/headers/musicplayer.h"
#include <dirent.h> 
#include <unistd.h> 


//Label and buttons made global so they can be dynamic 
GObject* currentSongLabel; 
GObject* titleLabel; 
GObject* artistLabel; 
GObject* albumLabel; 
GObject* yearLabel; 
GObject* repeatButton; 
GObject* pauseButton; 

//Animation 
GtkWidget *gifImage;
GdkPixbufAnimation *animation;
GdkPixbufAnimationIter *iter;

gboolean animation_playing = FALSE;


gboolean update_animation(gpointer data) {
    if (animation_playing) {
        gdk_pixbuf_animation_iter_advance(iter, NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(gifImage), gdk_pixbuf_animation_iter_get_pixbuf(iter));
    }
    return G_SOURCE_CONTINUE;
}

int main (int argc, char** argv) { 
    GError *error = NULL;

    gtk_init(&argc, &argv);

    /* Construct a GtkBuilder instance and load our UI description */
    GtkBuilder* builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "src/gui/builder.ui", &error) == 0)
    {
	g_printerr ("Error loading file: %s\n", error->message);
        g_error_free(error);
	exit(EXIT_FAILURE); 
    }

    /* Connect signal handlers to the constructed widgets. */
    GObject* window = gtk_builder_get_object (builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (quit), NULL);

    pauseButton = gtk_builder_get_object(builder, "pauseButton"); 
    g_signal_connect(pauseButton, "clicked", G_CALLBACK(pauseCallback), NULL); 

    GObject* button = gtk_builder_get_object(builder, "skipButton"); 
    g_signal_connect(button, "clicked", G_CALLBACK(skipCallback), NULL); 

    button = gtk_builder_get_object(builder, "prevButton"); 
    g_signal_connect(button, "clicked", G_CALLBACK(previousCallback), NULL); 

    repeatButton = gtk_builder_get_object(builder, "repeatButton"); 
    g_signal_connect(repeatButton, "clicked", G_CALLBACK(repeatCallback), NULL); 


    currentSongLabel = gtk_builder_get_object(builder, "currentSong"); 
    titleLabel = gtk_builder_get_object(builder, "songTitle"); 
    artistLabel = gtk_builder_get_object(builder, "songArtist"); 
    albumLabel = gtk_builder_get_object(builder, "songAlbum"); 
    yearLabel = gtk_builder_get_object(builder, "songYear"); 


    GObject* chooser_button = gtk_builder_get_object(builder, "playlistSelect");
    gtk_file_chooser_set_action(GTK_FILE_CHOOSER(chooser_button), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);

    g_signal_connect(chooser_button, "file-set", G_CALLBACK(onPlaylistSet), NULL);

    //sets the file chooser to start in the music directory
    const char* home = getenv("HOME");  // Get home directory
    
    if (home != NULL) {
        char *musicPath = g_build_filename(home, "Music", NULL);
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(chooser_button), musicPath);
	runQueueThread(musicPath); // if the music folder does not exist this will end 
	//Cant free the music path as it needs to be used first in runQueueThread
    }

    //Animation 
    gifImage = GTK_WIDGET(gtk_builder_get_object(builder, "gifImage"));
    animation = gdk_pixbuf_animation_new_from_file("images/kiryuDancing.gif", &error);
    if (error != NULL) {
        g_warning("Failed to load GIF animation: %s", error->message);
        g_error_free(error);
	exit(EXIT_FAILURE); 
    }
    iter = gdk_pixbuf_animation_get_iter(animation, NULL);

    g_timeout_add(100, update_animation, NULL);
    animation_playing = TRUE;


    gtk_main();
    g_object_unref(animation);

    return 0;
}

