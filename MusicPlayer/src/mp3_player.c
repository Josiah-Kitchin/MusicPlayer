#include <stdio.h>
#include <stdlib.h>
#include <mpg123.h>
#include <ao/ao.h>

#define BITS_PER_SAMPLE 16
#define NUM_CHANNELS 2
#define SAMPLE_RATE 44100

void playMp3File(const char *mp3_file) {
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int driver_id;
    ao_device *device;
    int format, channels, encoding;
    long rate;

    // Initialize mpg123
    mpg123_init();
    mh = mpg123_new(NULL, NULL);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    // Open the MP3 file
    mpg123_open(mh, mp3_file);

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
    driver_id = ao_default_driver_id();
    device = ao_open_live(driver_id, &ao_format, NULL);

    // Decode and play MP3
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
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

