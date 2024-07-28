

#include "../headers/song_queue.hpp"
#include <iostream> 
#include <gtest/gtest.h> 

using namespace std; 

SongInfo getSongInfo(const string); 

TEST(metadata, getSongInfo) { 
    SongInfo actual = getSongInfo("../test_playlist/meta_test.mp3"); 
    SongInfo expected("B.Y.O.B.", "System of a Down", "Mezmerize", "Rock", 2005); 
    expected.hasCover = true; 
    
    EXPECT_EQ(actual.title, expected.title); 
    EXPECT_EQ(actual.genre, expected.genre); 
    EXPECT_EQ(actual.album, expected.album); 
    EXPECT_EQ(actual.year, expected.year); 
    EXPECT_EQ(actual.artist, expected.artist); 
    EXPECT_EQ(actual.hasCover, expected.hasCover); 
}

TEST(metadata, getSongInfoEmpty) { 
    SongInfo actual = getSongInfo("../test_playlist/test.mp3"); 
    SongInfo expected("", "", "", "", -1); 
    expected.hasCover = false; 

    EXPECT_EQ(actual.title, expected.title); 
    EXPECT_EQ(actual.genre, expected.genre); 
    EXPECT_EQ(actual.album, expected.album); 
    EXPECT_EQ(actual.year, expected.year); 
    EXPECT_EQ(actual.artist, expected.artist); 
    EXPECT_EQ(actual.hasCover, expected.hasCover); 
}


