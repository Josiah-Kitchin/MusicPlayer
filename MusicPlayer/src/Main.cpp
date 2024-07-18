#include <iostream> 
#include <string> 
#include <cstdlib>
#include <gtkmm.h> 
#include <thread> 

#include "headers/GuiFunctions.h" 
#include "headers/SongQueue.h"
using namespace std; 



int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "music.player");

    Gtk::Window* mainWindow = setUpGui(); 

    app->run(*mainWindow);
    if (songQueue.isPaused) {  //so the thread can end
	songQueue.togglePause(); 
    }
    if (playerThreadBegan) {    
	songQueue.exit(); 
	playerThread.join(); 
    }

    return 0;
}

