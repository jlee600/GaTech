#ifndef SPOTIFY_H
#define SPOTIFY_H

/* DO NOT MODIFY FILE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x) ((void)x)
#define FAILURE 0
#define SUCCESS 1

#define INCOMPLETE -1
#define INCOMPLETE_PTR NULL

#include "suites/fakemalloc.h"

typedef enum Genre {
    COUNTRY,
    POP,
    ROCK,
    LOFI,
    METAL
} Genre;

typedef struct Song {
    char *name;
    char *artist;
    Genre genre;
} Song;

typedef struct SongNode {
    Song song;
    struct SongNode *next;
} SongNode;

typedef struct Playlist {
    char *name;
    SongNode *head;
} Playlist;

// Initializers
int init_playlists(int initial_capacity);
int create_playlist(char *name);

// Playlist operations
int add_song_to_playlist(char *name, char *artist, Genre genre, Playlist *playlist);
void remove_song_from_playlist(Song *song, Playlist *playlist);
int playlist_contains_song(Song *song, Playlist *playlist);

// Queue operations
int queue_song(Song *song);
int queue_playlist(char *name);
void play_next(void);

// Bulk playlist operations
int create_playlist_of_genre(char *name, Genre genre);
int copy_songs_from_artist(Playlist *src, Playlist *dest, char *artist);

// Destroying
void destroy_playlist(int index);
void destroy_all_playlists(void);
void destroy_queue(void);

#endif
