
#include "../headers/audio.hpp"
#include <gtest/gtest.h> 


TEST(audiofilefuncs, getAudioType) { 

    EXPECT_EQ(MP3, getAudioType("test.mp3")); 
    EXPECT_EQ(MP3, getAudioType(".mp3")); 
    EXPECT_EQ(INVALID, getAudioType("test.mp2")); 
    EXPECT_EQ(INVALID, getAudioType("test.")); 
    EXPECT_EQ(INVALID, getAudioType("test.2")); 
    EXPECT_EQ(INVALID, getAudioType("")); 
}

    



