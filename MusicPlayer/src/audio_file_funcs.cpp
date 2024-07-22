#include <iostream> 
#include <string> 
#include "headers/audio.hpp" 
#include "headers/song_queue.hpp"

/* Functions used for parsing audio files */

typedef enum { 
    MP3, 
    INVALID
} AudioType; 


AudioType getAudioType(const std::string audioFile) { 
    //Returns the audio type of a string.  
    if (audioFile.length() < 3) { 
	return INVALID; 
    }
    if (audioFile.substr(audioFile.length() - 4) == ".mp3") { 
	return MP3; 
    }
    return INVALID; 
}

void playAudioFile(SongQueue* songQueue, const std::string audioFilePath) { 
    switch(getAudioType(audioFilePath)) { 
	case MP3: 
	    playMp3(songQueue, audioFilePath); 
	    break; 
	default: 
	    std::cout << "Invalid type" << std::endl; 
    }
}

std::string getMusicPath() { 
    const char* home = getenv("HOME"); 
    std::string homePath(home); 
    std::string musicPath = homePath + "/Music";
    return musicPath; 
}

std::string getHomePath() { 
    const char* home = getenv("Home"); 
    std::string homePath(home); 
    return homePath; 
}




