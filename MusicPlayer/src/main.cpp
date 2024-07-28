
/* A downloaded music player application
 * By Josiah Kitchin, Computer Science at the University of Oregon 
 *
 * Uses MVC architecture
 * Only supports MP3 files at this time
 * Select a playlist and play some music 
 *
 */


#include <gtkmm.h> 

#include "headers/view.hpp"
#include "headers/controller.hpp"
#include "headers/song_queue.hpp"

using namespace std; 


int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "music.player");
    PlayerView view("../src/gui/builder.ui"); 
    SongQueue songQueue; 
    PlayerController controller(songQueue, view);   
    songQueue.setController(&controller); 
    controller.connectSignals(); 

    app->run(*(view.mainWindow));

    

    return 0;
}

