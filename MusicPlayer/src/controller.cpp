
/*
 * The controller in MVC architecture
 * Sets the signals of all gui buttons to methods of the SongQueue class (model) and sends the 
 * approiate event to the PlayerView class. 
 *
 * The controller also manages the audioThread, which plays concurrently with the gui
 * as soon as a playlist is chosen. 
 */

#include <thread> 
#include <string> 
#include <gtkmm.h> 
#include "headers/controller.hpp"
#include "headers/view.hpp"
#include "headers/song_queue.hpp"
#include "headers/audio.hpp" 
#include "headers/events.hpp" 
 
using namespace std; 

PlayerController::PlayerController(SongQueue& songQueue, PlayerView& view) : songQueue(songQueue), view(view){
    audioThreadBegan = false; 
}


void PlayerController::handlePauseSignal() { 
    songQueue.togglePause(); 
    PlayerEvent event; 
    if (songQueue.isPaused) { 
	event = PlayerEvent::pauseSong; 
    } else { 
	event = PlayerEvent::playSong; 
    }
    view.togglePause(event); 
}

void PlayerController::handleNextSignal() { 
    songQueue.skip();   
}

void PlayerController::handlePreviousSignal() { 
    songQueue.previous();   
}

void PlayerController::handleRepeatSignal() { 
    songQueue.toggleRepeat(); 
    PlayerEvent event; 
    if (songQueue.isRepeat) { 
	event = PlayerEvent::repeatEnabled; 
    } else { 
	event = PlayerEvent::repeatDisabled; 
    }
    view.toggleRepeat(event); 
}

void PlayerController::handlePlaylistChooserSignal() { 
    string playlistPath = view.playlistChooser->get_filename(); 
    setPlaylist(playlistPath); 
}


void PlayerController::handleSongInfo(SongInfo songInfo) {
    view.updateSongInfo(songInfo); 
}


void PlayerController::connectSignals() {
    view.pauseButton->signal_clicked().connect([&](){this->handlePauseSignal();}); 
    view.nextButton->signal_clicked().connect([&](){this->handleNextSignal();}); 
    view.previousButton->signal_clicked().connect([&](){this->handlePreviousSignal();}); 
    view.repeatButton->signal_clicked().connect([&](){this->handleRepeatSignal();}); 

    view.playlistChooser->set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    view.playlistChooser->set_current_folder(getMusicPath()); 
    view.playlistChooser->signal_file_set().connect([&](){this->handlePlaylistChooserSignal();}); 
}

void PlayerController::setPlaylist(const string playlistPath) { 
    if (audioThreadBegan) { 
	bool wasPaused = false; 
	if (songQueue.isPaused) {  //This strategy causes it to play for a small time, must fix later
	    songQueue.isPaused = false; 
	    wasPaused = true; 
	}
	songQueue.exit(); 
	audioThread.join(); 
	if (wasPaused) { 
	    songQueue.isPaused = true; 
	}
    }
    audioThreadBegan = true; 
    audioThread = thread([=](){
	songQueue.clear(); 
	songQueue.enqueueSongsFromPath(playlistPath); 
	songQueue.beginQueue(""); 
    });
}

void PlayerController::exit() { 
    if (songQueue.isPaused) {  //so the thread can end
	songQueue.togglePause(); 
    }
    if (audioThreadBegan) {    
	songQueue.exit(); 
	audioThread.join(); 
    }
}


