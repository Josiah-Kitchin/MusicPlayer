
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sndfile.h>
#include <ao/ao.h>
#include <unistd.h> 

#include "../headers/musicplayer.h"

//Not currently supported

#define BITS 16

void playWavFile(const char* wav_file) {
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    ao_device *device;
    ao_sample_format format;
    int driver_id;

    // Initialize libsndfile
    sndfile = sf_open(wav_file, SFM_READ, &sfinfo);
    if (!sndfile) {
        fprintf(stderr, "Error: Couldn't open WAV file '%s'\n", wav_file);
        return;
    }

    // Initialize libao
    ao_initialize();

    // Setup format
    format.bits = BITS;
    format.channels = sfinfo.channels;
    format.rate = sfinfo.samplerate;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = NULL;

    // Choose driver
    driver_id = ao_default_driver_id();

    // Open driver
    device = ao_open_live(driver_id, &format, NULL /* no options */);
    if (device == NULL) {
        fprintf(stderr, "Error: Unable to open audio device\n");
        sf_close(sndfile);
        ao_shutdown();
        return;
    }

    // Read and play audio
    int read_count;
    sf_count_t buffer_size = sfinfo.channels * sfinfo.frames;
    short *buffer = (short *) malloc(buffer_size * sizeof(short));
    while ((read_count = sf_read_short(sndfile, buffer, buffer_size)) > 0) {
	if (skip) { 
	    skip = !skip;
	    break; 
	}
	while (paused) { 
	    usleep(5000); 
	}
	ao_play(device, (char *) buffer, read_count * sizeof(short));
    }

    // Cleanup
    free(buffer);
    ao_close(device);
    sf_close(sndfile);
    ao_shutdown();
}
