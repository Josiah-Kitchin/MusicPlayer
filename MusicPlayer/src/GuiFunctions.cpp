
#include <iostream> 
#include <string> 
#include <cstdlib>
#include <gtkmm.h> 
#include <thread> 
#include <string> 
#include <mutex> 

#include "headers/SongQueue.h"
#include "headers/Audio.h"
using namespace std; 

//Global song queue, flags can be changed from other files. 
SongQueue songQueue; 
bool playerThreadBegan = false; 

thread playerThread;

void changePlaylist(const string playlistPath){
    //Joins the current player thread (if it began), clears the current queue
    //then enqueues songs from the given playlist path 
    if (playerThreadBegan) { 
	bool wasPaused = false; //Pausing prevents the queue from exiting, so must unpause briefly
	if (songQueue.isPaused) {  //This strategy causes it to play for a small time, must fix later
	    songQueue.isPaused = false; 
	    wasPaused = true; 
	}
	songQueue.exit(); 
	playerThread.join(); 
	if (wasPaused) { 
	    songQueue.isPaused = true; 
	}
    }
     
    playerThreadBegan = true; 
    playerThread = thread([=](){
	songQueue.clear(); 
	songQueue.enqueueSongsFromPath(playlistPath); 
	songQueue.beginQueue(""); 
    });
}

Gtk::Label* titleLabel; 
Gtk::Label* artistLabel; 
Gtk::Label* albumLabel; 
Gtk::Label* genreLabel; 
Gtk::Label* yearLabel; 
Gtk::Image* albumCover; 

void updateSongInfoLabels(SongInfo songInfo) { 
    titleLabel->set_text(songInfo.title); 
    artistLabel->set_text(songInfo.artist); 
    albumLabel->set_text(songInfo.album); 
    genreLabel->set_text(songInfo.genre); 
    if (songInfo.year <= 0) { 
	yearLabel->set_text(""); 
    } else { 
	yearLabel->set_text(to_string(songInfo.year)); 
    }
    if (songInfo.hasCover) {
	songInfo.cover = songInfo.cover->scale_simple(200, 200, Gdk::INTERP_BILINEAR); // set the size
	albumCover->set(songInfo.cover); 
    } else { 
	auto noAlbumCover = Gdk::Pixbuf::create_from_file("src/images/noAlbumCover.jpeg");
	noAlbumCover = noAlbumCover->scale_simple(200, 200, Gdk::INTERP_BILINEAR);
	albumCover->set(noAlbumCover); 
    }
}

	
Gtk::Window* setUpGui() { 
    //Connects all of the signals and sets the GUI logic
    //Returns the main window to run in Main.cpp

    auto builder = Gtk::Builder::create(); 
    builder->add_from_file("src/gui/builder.ui"); 
    if (!builder) { 
	cerr << "Error adding builder" << endl; 
	exit(EXIT_FAILURE); 
    }

    Gtk::Window* mainWindow = nullptr; 
    builder->get_widget("mainWindow", mainWindow); 
    if (!mainWindow) { 
	cerr << "Can not find the main window from UI file" << endl; 
	exit(EXIT_FAILURE); 
    }

    //Playlist chooser

    Gtk::FileChooserButton* fileChooserButton = nullptr; 
    builder->get_widget("playlistChooser", fileChooserButton); 

    fileChooserButton->set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    fileChooserButton->set_current_folder(getMusicPath()); 

    if (fileChooserButton) { 
	fileChooserButton->signal_file_set().connect([=](){
	    string playlistPath = fileChooserButton->get_filename(); 
	    changePlaylist(playlistPath); 
	});
    }

    //Pause button
    Gtk::Button* button = nullptr; 
    builder->get_widget("pauseButton", button); 
    if (button) { 
	button->signal_clicked().connect([=](){
	    songQueue.togglePause(); 
	    if (songQueue.isPaused) { 
		button->set_image_from_icon_name("media-playback-start"); 
		button->set_label("Play"); 
	    } else { 
		button->set_image_from_icon_name("media-playback-pause"); 
		button->set_label("Pause"); 
	    }
	}); 
    }  
    
    //Skip button
    builder->get_widget("nextButton", button); 
    if (button) { 
	button->signal_clicked().connect([]() {
	    songQueue.skip(); 
	}); 
    }

    builder->get_widget("repeatButton", button); 
    if (button) { 
	button->signal_clicked().connect([=](){
	    songQueue.toggleRepeat(); 
	    if (songQueue.isRepeat) { 
		button->set_label("Disable Repeat"); 
	    } else { 
		button->set_label("Enable Repeat"); 
	    }
	}); 
    }

    builder->get_widget("previousButton", button); 
    if (button) { 
	button->signal_clicked().connect([](){
	    songQueue.previous(); 
	}); 
    }

    //set the global labels 
    builder->get_widget("title", titleLabel); 
    builder->get_widget("artist", artistLabel); 
    builder->get_widget("album", albumLabel); 
    builder->get_widget("genre", genreLabel); 
    builder->get_widget("year", yearLabel); 
    builder->get_widget("albumCover", albumCover); 

    auto noAlbumCover = Gdk::Pixbuf::create_from_file("src/images/noAlbumCover.jpeg");
    noAlbumCover = noAlbumCover->scale_simple(200, 200, Gdk::INTERP_BILINEAR);
    albumCover->set(noAlbumCover); 


    return mainWindow; 
}

