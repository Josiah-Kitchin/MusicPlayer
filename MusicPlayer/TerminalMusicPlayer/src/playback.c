


#include "headers/musicplayer.h" 

void togglePause() { 
    paused = !paused; 
}

void skipSong() { 
    skip = !skip; 
}

void previousSong() { 
    skip = !skip; 
    playPrevious = !playPrevious; 
}

void toggleRepeat() { 
    repeat = !repeat;    
}

