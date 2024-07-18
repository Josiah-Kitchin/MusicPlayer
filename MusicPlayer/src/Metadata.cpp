
#include <iostream>
#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <fstream> 
#include <cstdlib> 
#include <glibmm.h> 
#include <gtkmm.h> 

#include "headers/SongQueue.h"


using namespace std; 

Glib::RefPtr<Gdk::Pixbuf> convertDataToPixBuf(TagLib::ByteVector coverData) { 
    Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
    loader->write(reinterpret_cast<const unsigned char*>(coverData.data()), coverData.size());
    loader->close();
    return loader->get_pixbuf();
}


SongInfo getSongInfo(const string audioFilePath) { 

    TagLib::MPEG::File file(audioFilePath.c_str()); 
    TagLib::ID3v2::Tag* tag = file.ID3v2Tag();
    if (!tag) {
        cerr << "No ID3v2 tag found in the file." << endl;
	SongInfo emptySongInfo("Unknown", "", "", "", -1);  
	return emptySongInfo;  
    }

    // Retrieve basic metadata
    string title = tag->title().toCString(true);  // true: UTF-8
    string artist = tag->artist().toCString(true);
    string album = tag->album().toCString(true);
    string genre = tag->genre().toCString(true);
    int year = tag->year();

    SongInfo songInfo(title, artist, album, genre, year); 

    // Retrieve album cover (if available)
    TagLib::ID3v2::FrameList frames = tag->frameListMap()["APIC"];
    if (!frames.isEmpty()) {
        TagLib::ID3v2::AttachedPictureFrame* coverFrame =
            static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
        
    TagLib::ByteVector coverData = coverFrame->picture();
	songInfo.hasCover = true; 
	songInfo.cover = convertDataToPixBuf(coverData); 
    } 
    return songInfo;
}





