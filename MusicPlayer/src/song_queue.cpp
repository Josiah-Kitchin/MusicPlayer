
/*
 * The SongQueue class acts as the model in the MVC architecture. Holds songs from a playlist
 * in a doubly linked list. Has methods for playback features. 
 * A pointer to the controller is stored in this class so the song queue can speak to it. 
 */


#include <iostream> 
#include <string> 
#include <memory> 
#include <filesystem> 
#include <vector> 
#include <glibmm.h> 
#include <gtkmm.h> 

#include "headers/song_queue.hpp"
#include "headers/audio.hpp"
#include "headers/controller.hpp" 

using namespace std; 
namespace fs = std::filesystem;

Song::Song(const string songPath) : songPath(songPath), nextSong(nullptr), prevSong(nullptr), 
				    info(getSongInfo(songPath)){}; 
				      

//No song cover at initailzatoin. The flag hasCover is to determine whether a song has a cover or not
SongInfo::SongInfo(const string title, const string artist, const string album, const string genre, 
	           int year) : title(title), artist(artist), album(album), genre(genre), year(year), hasCover(false){}

SongQueue::SongQueue() : headSong(nullptr), tailSong(nullptr), controller(nullptr) {

    isPlaying = false; 
    isPaused = true; 
    isSkipped = false; 
    isRepeat = false; 
    isPrevious = false; 
} 

void SongQueue::setController(PlayerController* inputController) { 
    //Input the memory address of the controller. The song queue is initialized first, so
    //that is why it can not be done at initialization
    controller = inputController; 
}
    
void SongQueue::enqueue(const string songPath) { 
    //Appends a song to the end of the linked list 
    shared_ptr<Song> newSong = make_shared<Song>(songPath);
    if (!headSong) { 
	headSong = newSong; 
	tailSong = newSong; 
    } else { 
	tailSong->nextSong = newSong; //Append new song to the end    
	newSong->prevSong = tailSong.get(); 
	tailSong = newSong; 
    }
}

void SongQueue::enqueueSongsFromPath(const string playlistPath) { 
    //Enqueues all files in a given directory path ( should be treated as a playlist) 
    try { 
	for (const auto& entry : fs::directory_iterator(playlistPath)) { 
	    enqueue(entry.path()); 
	}
    } catch (const fs::filesystem_error& ex) { 
	    cerr << "Error accessing directory: " << ex.what() << endl; 
    }
}

void SongQueue::beginQueue(const string songPath) {
    //Begins the song queue at the given songPath. If the song path is an empty string, 
    //it starts at the beggining 
    shared_ptr<Song> currentSong = headSong; 

    if (!songPath.empty()) { 
	while (currentSong && currentSong->songPath != songPath) { 
	    currentSong = currentSong->nextSong; 
	}
	if (!currentSong) { 
	    cerr << "Error beggining Queue: " << songPath << " not found" << endl;  
	    return; 
	}
    } 
    
    isPlaying = true; 
    while (currentSong && isPlaying) {  //isPlaying can be set to false to return the function 
	
	//let the controller handle communicating with the view for the song info
	controller->handleSongInfo(currentSong->info); 
	
	//audio players need a reference to the song queue to implement playback features
	playAudioFile(this, currentSong->songPath); 

	//If repeat is enabled and the queue is on the last song, restart at the head 
	if (isRepeat && !currentSong->nextSong) { 
	    currentSong = headSong; 
	    continue; 
	}

	if (isPrevious) { 
	    isPrevious = false; //Reset the previous flag 
	    if (!currentSong->prevSong) { 
		continue; //If the queue is on the first song, repeat. 
	    }
	    if (!currentSong->prevSong->prevSong) { 
		currentSong = headSong; 
		continue;
	    }
	    //This is pretty cursed, however it will double free if you try to create
	    //a shared pointer from the previous raw pointer 
	    currentSong = currentSong->prevSong->prevSong->nextSong; 
	    continue; 
	}
	currentSong = currentSong->nextSong; 
    }
    isPlaying = false; 
}

vector<string> SongQueue::getSongsInQueue() { 
    //Returns a vector of all songs in the queue (used for testing)
    shared_ptr<Song> currentSong = headSong; 
    vector<string> songs; 
    while (currentSong) {
	songs.push_back(currentSong->songPath); 
	currentSong = currentSong->nextSong; 
    }
    return songs; 
}

void SongQueue::clear() { 
    headSong = nullptr; 
    tailSong = nullptr; 
}


void SongQueue::togglePause() { 
    isPaused = !isPaused; 
}

void SongQueue::skip() { 
    if (isPlaying) { //prevents delayed skipping 
	isSkipped = true; 
    }
}

void SongQueue::toggleRepeat() { 
    isRepeat = !isRepeat; 
}

void SongQueue::previous() { 
    isPrevious = true; 
    skip(); 
}

void SongQueue::exit() { 
    isPlaying = false; 
    isSkipped = true;  // makes sure it stops playing whatever song the queue is on  
}



    












