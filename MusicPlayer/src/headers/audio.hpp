


#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <string> 
#include <gtkmm.h> 
#include <glibmm.h> 

#include "song_queue.hpp" 

//players.cpp
typedef enum { 
    MP3, 
    INVALID
} AudioType; 

AudioType getAudioType(const std::string); 
void playMp3(SongQueue*, const std::string); 

//audiofilefuncs.cpp
void playAudioFile(SongQueue*, const std::string); 
std::string getMusicPath(); 
std::string getHomePath(); 


//Metadata.cpp
SongInfo getSongInfo(const std::string audioFilePath);  

#endif


