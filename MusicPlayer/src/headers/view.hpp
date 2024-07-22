


#ifndef VIEW_HPP 
#define VIEW_HPP 

#include <vector> 
#include <string> 
#include <gtkmm.h> 

#include "events.hpp" 


struct SongInfo; 

class PlayerView{ 
public: 
    Gtk::Window* mainWindow; 
    Gtk::FileChooserButton* playlistChooser; 

    Gtk::Button* pauseButton; 
    Gtk::Button* previousButton; 
    Gtk::Button* nextButton; 
    Gtk::Button* repeatButton; 

    Gtk::Label* titleLabel;
    Gtk::Label* artistLabel;
    Gtk::Label* albumLabel;
    Gtk::Label* genreLabel;
    Gtk::Label* yearLabel;

    Gtk::Image* albumCover; 

    PlayerView(const std::string builder); 
    void togglePause(PlayerEvent); 
    void toggleRepeat(PlayerEvent); 
    void updateSongInfo(SongInfo); 

};

#endif
