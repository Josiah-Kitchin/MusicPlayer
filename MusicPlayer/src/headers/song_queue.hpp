

#ifndef SONG_QUEUE_HPP
#define SONG_QUEUE_HPP

#include <cstring> 
#include <memory> 
#include <string> 
#include <vector> 
#include <glibmm.h> 
#include <gtkmm.h> 
#include "controller.hpp" 

//song_queue.cpp

struct SongInfo { 
    std::string title; 
    std::string artist; 
    std::string album; 
    std::string genre; 
    int year; 
    Glib::RefPtr<Gdk::Pixbuf> cover; 
    SongInfo(const std::string title, const std::string artist, const std::string album, 
	     const std::string genre, int year);  
    bool hasCover; 
};

struct Song { 
    std::shared_ptr<Song> nextSong; 
    Song* prevSong; 
    std::string songPath;
    SongInfo info; 
    Song(const std::string songPath); 
};

class SongQueue { 
private: 
    std::shared_ptr<Song> headSong;
    std::shared_ptr<Song> tailSong; 
    PlayerController* controller; 
public: 
    //Flags
    bool isPlaying; 
    bool isPaused; 
    bool isSkipped; 
    bool isRepeat; 
    bool isPrevious; 
    
    SongQueue();
    void setController(PlayerController*); 
    void enqueue(const std::string songPath); 
    void enqueueSongsFromPath(const std::string playlistPath); 
    void beginQueue(const std::string songPath); 
    std::vector<std::string> getSongsInQueue(); 
    


    void clear(); 
    void exit(); 
    
    //Playback 
    void togglePause(); 
    void skip(); 
    void toggleRepeat(); 
    void previous(); 
    
};

#endif



