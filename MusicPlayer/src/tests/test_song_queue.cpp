


#include "../headers/song_queue.hpp"
#include <iostream> 
#include <gtest/gtest.h> 
#include <vector> 
#include <algorithm> 

using namespace std; 


TEST(SongQueue, enqueue) {
    SongQueue sq; 
    sq.enqueue("song1"); 
    sq.enqueue("song2"); 
    sq.enqueue("song3"); 
    sq.enqueue("song4"); 
    sq.enqueue("song5"); 

    vector<string> expected = {"song1", "song2", "song3", "song4", "song5"}; 
    vector<string> actual = sq.getSongsInQueue(); 
    EXPECT_EQ(expected, actual); 
}

TEST(SongQueue, enqueueSongsFromPath) { 
    SongQueue sq; 
    sq.enqueueSongsFromPath("../test_playlist"); 

    vector<string> expected = {"../test_playlist/test1.mp3", "../test_playlist/test2.mp3", "../test_playlist/test3.mp3",
			       "../test_playlist/meta_test.mp3"}; 
    vector<string> actual = sq.getSongsInQueue(); 
    sort(expected.begin(), expected.end()); 
    sort(actual.begin(), actual.end()); 

    EXPECT_EQ(expected, actual); 
}













