
/* Functions for playing audio files 
 * Playback features are implemented here using flags from the song queue
 *
 * */ 

#include <iostream>
#include <mpg123.h>
#include <ao/ao.h>
#include <cstring>
#include <fstream>
#include <string> 
#include <chrono> 
#include <thread>

#include "headers/song_queue.hpp" 

void playMp3(SongQueue* songQueue, const std::string audioFile) {
    // Initialize mpg123 library
    const char* cAudioFile = audioFile.c_str(); 
    mpg123_init();
    mpg123_handle *mh;

    // Open the file and get the format information
    mh = mpg123_new(nullptr, nullptr);
    mpg123_open(mh, cAudioFile);
    int channels, encoding;
    long rate;
    mpg123_getformat(mh, &rate, &channels, &encoding);

    // Initialize the ao library
    ao_initialize();
    ao_sample_format format;
    format.bits = mpg123_encsize(encoding) * 8;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = nullptr;

    // Open the default driver
    ao_device *device = ao_open_live(ao_default_driver_id(), &format, nullptr);

    // Read and play the file
    size_t buffer_size = 4096;
    unsigned char* buffer = new unsigned char[buffer_size];
    size_t done;
    int err;

    do {
	//Skip functionality 
	if (songQueue->isSkipped) { 
	    songQueue->isSkipped = false; 
	    break; 
	}
	//Pause functionality
	while (songQueue->isPaused) { 
	    std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

        err = mpg123_read(mh, buffer, buffer_size, &done);
        ao_play(device, (char*)buffer, done);
    } while (err == MPG123_OK);

    // Clean up
    delete[] buffer;
    ao_close(device);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}
