#include <stdio.h>
#include <stdlib.h>
#include <mpg123.h>
#include <ao/ao.h>
#include <unistd.h> 

#include "../headers/musicplayer.h"


#define BITS_PER_SAMPLE 16
#define NUM_CHANNELS 2
#define SAMPLE_RATE 44100

void fillMp3SongInfo(const char* mp3_file) { 
    //Update the current song struct with id3 metadata
    mpg123_init();
    mpg123_handle* mh = mpg123_new(NULL, NULL);
    if (!mh) { 
	fprintf(stderr, "Unable to open mpg handle\n"); 
	return; 
    }

    if (mpg123_open(mh, mp3_file) != MPG123_OK) {
        fprintf(stderr, "Cannot open %s: %s\n", mp3_file, mpg123_strerror(mh));
        mpg123_delete(mh);
        mpg123_exit();
        return;
    }

    mpg123_id3v1* id3v1;
    mpg123_id3v2* id3v2;
    //Currently only id3v1 is implemented. 
    if (mpg123_id3(mh, &id3v1, &id3v2) == MPG123_OK) {
	if (currentSong.title != NULL) free(currentSong.title); 
	if (currentSong.artist != NULL) free(currentSong.artist); 
	if (currentSong.album != NULL) free(currentSong.album); 
	if (currentSong.year != NULL) free(currentSong.year); 
	if (id3v1) { 
	    if (id3v1->title) { currentSong.title = strdup(id3v1->title); }  
	    else { currentSong.title = strdup("Unknown Title"); }  
	    if (id3v1->artist) { currentSong.artist = strdup(id3v1->artist); }
	    else { currentSong.artist = strdup("Unknown Artist"); }  
	    if (id3v1->album) { currentSong.album = strdup(id3v1->album); }  
	    else { currentSong.album = strdup("Unknown Album"); }  
	    if (id3v1->year) { currentSong.year = strdup(id3v1->year); } 
	    else { currentSong.year = strdup("Unknown Year"); }  
	} else { 
	    currentSong.title = strdup("Unknown Title");    
	    currentSong.artist = strdup("Unknown Artist");    
	    currentSong.album = strdup("Unknown Album");    
	    currentSong.year = strdup("Unknown Year");    
	}
    }

    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}



void playMp3File(const char *mp3_file) {

    // Initialize mpg123
    mpg123_init();
    mpg123_handle* mh = mpg123_new(NULL, NULL);
    if (!mh) { 
	fprintf(stderr, "Unable to open mpg handle\n"); 
	return; 
    }
    size_t buffer_size = mpg123_outblock(mh);
    unsigned char* buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
    if (!buffer) { 
	fprintf(stderr, "Unable to allocate memory for audio buffer\n"); 
	return; 
    }

    // Open the MP3 file

    if (mpg123_open(mh, mp3_file) != MPG123_OK) {
        fprintf(stderr, "Cannot open %s: %s\n", mp3_file, mpg123_strerror(mh));
        mpg123_delete(mh);
        mpg123_exit();
        return;
    }

    int format, channels, encoding;
    long rate;

    // Get format information
    mpg123_getformat(mh, &rate, &channels, &encoding);

    // Initialize libao
    ao_initialize();

    // Setup format for libao
    format = (encoding == MPG123_ENC_SIGNED_16) ? AO_FMT_NATIVE : AO_FMT_LITTLE;
    ao_sample_format ao_format;
    ao_format.bits = mpg123_encsize(encoding) * 8;
    ao_format.channels = channels;
    ao_format.rate = rate;
    ao_format.byte_format = format;
    ao_format.matrix = NULL;

    // Choose driver and open device
    int driver_id = ao_default_driver_id();
    ao_device* device = ao_open_live(driver_id, &ao_format, NULL);

    // Decode and play MP3
    size_t done; 
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
	if (skip) { //Ends the for loop if skip, which is changed via signals
	    skip = !skip; 
	    break; 
	}
	while (paused) { //Endless loop of sleeping while paused, which is changed via signals 
	    usleep(1000); 
	}
	ao_play(device, (char *) buffer, done);
    }

    // Cleanup
    free(buffer);
    ao_close(device);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}

