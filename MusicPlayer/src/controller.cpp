
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
#include <filesystem> 
#include <iostream> 

 
using namespace std; 

PlayerController::PlayerController(SongQueue& songQueue, PlayerView& view) : songQueue(songQueue), view(view){
    audioThreadBegan = false; 
    //Maybe find a more organized way to deal with dispatchers. 
    //Allows gui to be updated while staying in the main thread
    dispatcher.connect([this] {
	handleSongInfo(this->songQueue.currentSongInfo); 
    });
}

PlayerController::~PlayerController() { 
    //exits the song queue and joins the audio thread 
    if (songQueue.isPaused) {  
	songQueue.togglePause(); 
    }
    if (audioThreadBegan) {    
	songQueue.exit(); 
	audioThread.join(); 
    }
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

void PlayerController::handleShuffleSignal() { 
    songQueue.toggleShuffle(); 
    PlayerEvent event; 
    if (songQueue.isShuffle) { 
	event = PlayerEvent::shuffleEnabled; 
    } else { 
	event = PlayerEvent::shuffleDisabled; 
    }
    view.toggleShuffle(event); 
}


void PlayerController::signalSongChange() { 
    //Because song queue runs in a different thread, it emits the signal from the dispatcher, 
    //sending the signal to the main thread and allowing the main thread to handle updating the song 
    //info display.
    dispatcher.emit(); 
}
void PlayerController::handleSongInfo(SongInfo songInfo) {
    view.updateSongInfo(songInfo); 
}

void PlayerController::displaySongSelection(const string playlistPath) { 
    //Displays the song selecter from the playlist path (from the file chooser) and 
    //connects the song change button signals
    namespace fs = std::filesystem; 

    //Remove rows from previous playlist song list 
    for (const auto& row : view.songList->get_children()) { 
	view.songList->remove(*row); 
    }

    for (const auto& file : fs::directory_iterator(playlistPath)) { 
	const string fileName = file.path().filename().string();
	if (getAudioType(fileName) == INVALID) { 
	    continue; 
	}
	Gtk::ListBoxRow* row = Gtk::manage(new Gtk::ListBoxRow());
	Gtk::Button* fileNameButton = Gtk::manage(new Gtk::Button(fileName)); 

	//add the css class to the buttons 
	auto style_context = fileNameButton->get_style_context(); 
	style_context->add_class("songSelectButton"); 
	
	row->add(*fileNameButton);
	view.songList->append(*row);

	fileNameButton->signal_clicked().connect([=]() { // get the strings by value or bad things will happen 
	    setPlaylist(playlistPath, playlistPath + "/" + fileName);
	});


    }
    view.mainWindow->show_all_children(); //neccessary to show the new rows 
}

void PlayerController::handlePlaylistChooserSignal() { 
    string playlistPath = view.playlistChooser->get_filename(); 
    setPlaylist(playlistPath, ""); 
    displaySongSelection(playlistPath); 
}


void PlayerController::connectSignals() {
    view.pauseButton->signal_clicked().connect([&](){this->handlePauseSignal();}); 
    view.nextButton->signal_clicked().connect([&](){this->handleNextSignal();}); 
    view.previousButton->signal_clicked().connect([&](){this->handlePreviousSignal();}); 
    view.repeatButton->signal_clicked().connect([&](){this->handleRepeatSignal();}); 
    view.shuffleButton->signal_clicked().connect([&](){this->handleShuffleSignal();}); 

    view.playlistChooser->set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    view.playlistChooser->set_current_folder(getMusicPath()); 
    view.playlistChooser->signal_file_set().connect([&](){this->handlePlaylistChooserSignal();}); 

    view.songList->set_selection_mode(Gtk::SELECTION_NONE);

}


void PlayerController::setPlaylist(const string playlistPath, const string songPath) { 
    //Controls the audio thread, for setting the playlist or changing a song in a playlist
    //The current thread ends and a new thread starts so we can change the song queue

    if (audioThreadBegan) { 
	//Ends the current thread
	if (songQueue.isPaused) {  
	    handleNextSignal(); 
	    handlePauseSignal(); 
	}
	songQueue.exit(); 
	audioThread.join(); 
    }
    
    audioThreadBegan = true; 
    audioThread = thread([=](){ //starts the song queue on a new thread 
	songQueue.clear(); 
	songQueue.enqueueSongsFromPath(playlistPath); 
	songQueue.begin(songPath); 
    });
}




