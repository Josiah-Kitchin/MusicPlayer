

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <thread> 
#include <string> 
#include "view.hpp" 

struct SongInfo; 
class SongQueue; 

class PlayerController { 
private: 
    SongQueue& songQueue; 
    PlayerView& view; 
    std::thread audioThread; 
    bool audioThreadBegan; 

public: 
    PlayerController(SongQueue&, PlayerView&);
    void handlePauseSignal();  
    void handleNextSignal();  
    void handlePreviousSignal();  
    void handleRepeatSignal();  
    void handlePlaylistChooserSignal();  
    void handleSongInfo(SongInfo); 
    void connectSignals(); 
    void setPlaylist(const std::string playlistPath); 
    void exit(); 
};

#endif



