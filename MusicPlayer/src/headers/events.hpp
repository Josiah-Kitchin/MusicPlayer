

#ifndef EVENTS_HPP
#define EVENTS_HPP

enum class PlayerEvent : int { 
    pauseSong, 
    playSong, 
    repeatEnabled, 
    repeatDisabled,
    shuffleEnabled, 
    shuffleDisabled
}; 

#endif
