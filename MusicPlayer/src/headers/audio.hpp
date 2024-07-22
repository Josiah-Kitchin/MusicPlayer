


#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <string> 
#include <gtkmm.h> 
#include <glibmm.h> 

#include "song_queue.hpp" 

//players.cpp
void playMp3(SongQueue*, const std::string); 

//audiofilefuncs.cpp
void playAudioFile(SongQueue*, const std::string); 
std::string getMusicPath(); 
std::string getHomePath(); 


//Metadata.cpp
SongInfo getSongInfo(const std::string audioFilePath);  

#endif


