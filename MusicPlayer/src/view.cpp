
/*
 * The view in MVC architecture
 * PlayerView holds the gui componenets from the builder file. It responds to events
 * and changes the gui accordingly.
 */

#include "headers/view.hpp" 
#include "headers/events.hpp" 
#include "headers/song_queue.hpp"
#include <string> 
#include <gtkmm.h> 
#include <iostream> 

using namespace std; 

PlayerView::PlayerView(const string builderPath) { 
    // get all the widgets that need signals or need to be changed dynamically here 
    auto builder = Gtk::Builder::create(); 
    builder->add_from_file(builderPath); 
    builder->get_widget("mainWindow", mainWindow); 

    builder->get_widget("pauseButton", pauseButton); 
    builder->get_widget("nextButton", nextButton); 
    builder->get_widget("previousButton", previousButton); 
    builder->get_widget("repeatButton", repeatButton); 
    builder->get_widget("playlistChooser", playlistChooser); 

    builder->get_widget("title", titleLabel); 
    builder->get_widget("artist", artistLabel); 
    builder->get_widget("album", albumLabel); 
    builder->get_widget("genre", genreLabel); 
    builder->get_widget("year", yearLabel); 
    builder->get_widget("albumCover", albumCover); 

    //default image
    auto noAlbumCover = Gdk::Pixbuf::create_from_file("src/images/noAlbumCover.jpeg");
    noAlbumCover = noAlbumCover->scale_simple(200, 200, Gdk::INTERP_BILINEAR);
    albumCover->set(noAlbumCover); 
}

void PlayerView::togglePause(PlayerEvent event) { 
    if (event == PlayerEvent::playSong) { 
	pauseButton->set_image_from_icon_name("media-playback-pause"); 
	pauseButton->set_label("Pause"); 
    } else if (event == PlayerEvent::pauseSong) { 
	pauseButton->set_image_from_icon_name("media-playback-start"); 
	pauseButton->set_label("Play"); 
    } else { 
	cerr << "Invalid PlayerEvent in togglePause" << endl; 
    }
}

void PlayerView::toggleRepeat(PlayerEvent event) { 
    if (event == PlayerEvent::repeatEnabled) { 
	repeatButton->set_label("Disable Repeat"); 
    } else if (event == PlayerEvent::repeatDisabled) { 
	repeatButton->set_label("Enable Repeat"); 
    } else { 
	cerr << "Invalid PlayerEvent in toggleRepeat" << endl; 
    }
}

void PlayerView::updateSongInfo(SongInfo songInfo) { 
    titleLabel->set_text(songInfo.title); 
    artistLabel->set_text(songInfo.artist); 
    albumLabel->set_text(songInfo.album); 
    genreLabel->set_text(songInfo.genre); 
    if (songInfo.year <= 0) { 
	yearLabel->set_text(""); 
    } else { 
	yearLabel->set_text(to_string(songInfo.year)); 
    }
    if (songInfo.hasCover) {
	songInfo.cover = songInfo.cover->scale_simple(200, 200, Gdk::INTERP_BILINEAR); // set the size
	albumCover->set(songInfo.cover); 
    } else { 
	auto noAlbumCover = Gdk::Pixbuf::create_from_file("src/images/noAlbumCover.jpeg");
	noAlbumCover = noAlbumCover->scale_simple(200, 200, Gdk::INTERP_BILINEAR);
	albumCover->set(noAlbumCover); 
    }
}















					       

