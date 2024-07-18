

#include <vector> 
#include <cstdlib> 
#include <iostream> 
#include <algorithm> 
#include <iterator> 
#include <thread> 

#include "../headers/SongQueue.h" 

using namespace std; 

SongQueue songQueue;

bool test_SongQueue() { 
    const char* home = getenv("HOME"); 
    string homePath(home); 
    string testPlaylist = homePath + "/Music/tests/"; 
    SongQueue sq; 
    sq.enqueueSongsFromPath(testPlaylist); 
    vector<string> expected = {testPlaylist + "test1.mp3", testPlaylist + "test2.mp3", 
			       testPlaylist + "testinvalid.fdj"}; 

    vector<string> actual = sq.getSongsInQueue(); 

    sort(expected.begin(), expected.end());
    sort(actual.begin(), actual.end());

    auto expectedSongIdx = expected.begin(); 
    auto actualSongIdx= actual.begin(); 

    for (; expectedSongIdx != expected.end() && actualSongIdx != actual.end(); ++expectedSongIdx, ++actualSongIdx) {
	cout << *expectedSongIdx << endl; 
	cout << *actualSongIdx << endl; 
        if (*expectedSongIdx != *actualSongIdx) { 
	    return false; 
	}
    }
    return true; 
}

void test_clearQueue()  { 
    songQueue.clearQueue(); 
}


int main() { 
     
    if (!test_SongQueue()) { 
	cout << "test_SongQueue Failed" << endl; 
    } else { 
	cout << "All Tests Passed" << endl; 
    }

    return 0; 
}
