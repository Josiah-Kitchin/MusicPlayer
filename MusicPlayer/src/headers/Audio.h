


#ifndef AUDIO_H
#define AUDIO_H

#include <string> 
#include <gtkmm.h> 
#include <glibmm.h> 

#include "SongQueue.h" 

//players.cpp
void playMp3(const std::string); 

//audiofilefuncs.cpp
void playAudioFile(const std::string); 
std::string getMusicPath(); 
std::string getHomePath(); 


//Metadata.cpp
SongInfo getSongInfo(const std::string audioFilePath);  




#endif


