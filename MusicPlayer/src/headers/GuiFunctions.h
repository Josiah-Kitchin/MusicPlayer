

#ifndef GUIFUNCTIONS_H
#define GUIFUNCTIONS_H

#include <thread>  
#include <string> 
#include <gtkmm.h> 
#include "SongQueue.h" 


Gtk::Window* setUpGui(); 
void changePlaylist(const std::string); 
void updateSongInfoLabels(SongInfo); 

extern std::thread playerThread; 
extern bool playerThreadBegan; 






#endif
