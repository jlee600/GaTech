#ifndef SPOTIFY_GLOBAL_H
#define SPOTIFY_GLOBAL_H
#include "../spotify.h"
#include "utils.h"

extern SongNode *queue;
extern Playlist **playlists;
extern int num_playlists;
extern int playlists_capacity;
extern int bytes_until_fail;

#endif