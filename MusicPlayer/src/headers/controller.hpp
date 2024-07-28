

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

    Glib::Dispatcher dispatcher; 
    

public: 
    PlayerController(SongQueue&, PlayerView&);
    ~PlayerController(); 
    void handlePauseSignal();  
    void handleNextSignal();  
    void handlePreviousSignal();  
    void handleRepeatSignal();  
    void handleShuffleSignal(); 
    void handlePlaylistChooserSignal();  
    void signalSongChange(); 
    void handleSongInfo(SongInfo songInfo); 
    void displaySongSelection(const std::string playlistPath); 
    void connectSignals(); 
    void setPlaylist(const std::string playlistPath, const std::string songPath); 
};

#endif



