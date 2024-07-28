


#ifndef VIEW_HPP 
#define VIEW_HPP 

#include <vector> 
#include <string> 
#include <gtkmm.h> 
#include <memory> 

#include "events.hpp" 


struct SongInfo; 

class PlayerView { 
public: 
    Gtk::Window* mainWindow; 
    Gtk::FileChooserButton* playlistChooser; 

    Gtk::Button* pauseButton; 
    Gtk::Button* previousButton; 
    Gtk::Button* nextButton; 
    Gtk::Button* repeatButton; 
    Gtk::Button* shuffleButton; 

    Gtk::Label* titleLabel;
    Gtk::Label* artistLabel;
    Gtk::Label* albumLabel;
    Gtk::Label* genreLabel;
    Gtk::Label* yearLabel;

    Gtk::Image* albumCover; 

    Gtk::ScrolledWindow* scrolledSongSelect; 
    Gtk::Viewport* songViewport; 
    Gtk::ListBox* songList; 

    


    PlayerView(const std::string builder); 
    void togglePause(PlayerEvent); 
    void toggleRepeat(PlayerEvent); 
    void toggleShuffle(PlayerEvent); 
    void updateSongInfo(SongInfo); 
    void displayAboutPopover(); 

    Glib::RefPtr<Gtk::CssProvider> css;  

};

#endif
