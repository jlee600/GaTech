/**
 * CS 2110 - Fall 2024 - Project 5
 *
 * @author Jinseo Lee
 */

#include "spotify.h"

// Global variables - do not change these!
SongNode *queue;
Playlist **playlists;
int num_playlists;
int playlists_capacity;

/** INITIALIZERS */

/** init_playlists
 *
 * Initializes playlists as a new, empty array list with capacity initial_capacity.
 *
 * @param initial_capacity the initial capacity of the playlists array list (the
 * initial number of playlists that can be stored). This must be at least 1.
 *
 * @return Returns FAILURE upon malloc failure or invalid initial capacity and SUCCESS 
 * otherwise
 */
int init_playlists(int initial_capacity) {
    if (initial_capacity < 1) { 
        return FAILURE;
    }
    playlists = malloc(initial_capacity * sizeof(Playlist *));
    if (!playlists) {
        return FAILURE;
    }
    num_playlists = 0; 
    playlists_capacity = initial_capacity;
    return SUCCESS;
}

/** create_playlist
 *
 * Creates a new instance of the Playlist struct with name and an empty linked
 * list of songs and adds it to the end of playlists, resizing the backing array of
 * playlists if necessary by doubling the capacity.
 *
 * @param name the name of the new playlist
 *
 * @return FAILURE upon playlists not initialized, malloc or realloc failure, or 
 * invalid input and SUCCESS otherwise
 */
int create_playlist(char *name) {
    if (!name || !playlists || playlists_capacity == 0) {
        return FAILURE;
    }
    if (num_playlists == playlists_capacity) {
        Playlist **new_PL = realloc(playlists, 2 * playlists_capacity * sizeof(Playlist *));
        if (!new_PL) {
            return FAILURE;
        }
        playlists = new_PL;
        playlists_capacity *= 2;
    }

    Playlist *new_playlist = malloc(sizeof(Playlist));
    if (!new_playlist || playlists_capacity < 1) {
        return FAILURE;
    }
    new_playlist->name = malloc(strlen(name) + 1);
    if (!new_playlist->name) { 
        free(new_playlist);
        return FAILURE;
    }
    strcpy(new_playlist->name, name);
    new_playlist->head = NULL; 

    playlists[num_playlists++] = new_playlist;
    return SUCCESS;
}

/** PLAYLIST OPERATIONS */

/** add_song_to_playlist
 *
 * Adds a new song with name name, artist artist, and genre genre to the end of
 * the linked list of songs in playlist. If the song already exists in playlist,
 * do not add it again and return FAILURE.
 *
 * @param name the name of the song
 * @param artist the name of the artist of the song
 * @param genre the genre of the song
 * @param playlist the playlist to add the song to
 *
 * @return FAILURE upon malloc failure, duplicate song, or invalid inputs, and SUCCESS otherwise
 */
int add_song_to_playlist(char *name, char *artist, Genre genre, Playlist *playlist) {
    if (!name || !artist || !playlist) { 
        return FAILURE;
    }
    SongNode *curr = playlist->head;
    while (curr) { 
        int compared1 = strcmp(curr->song.name, name);
        int compared2 = strcmp(curr->song.artist, artist);
        int compared3 = curr->song.genre == genre;
        if (compared1 == 0 && compared2 == 0 && compared3 == 1) {
            return FAILURE;
        }
        curr = curr->next;
    }
    SongNode *newSong = malloc(sizeof(SongNode));
    if (!newSong) {
        return FAILURE;
    }

    newSong->song.name = malloc(strlen(name) + 1);
    if (!newSong->song.name) { 
        free(newSong);
        return FAILURE;
    }
    strcpy(newSong->song.name, name);

    newSong->song.artist = malloc(strlen(artist) + 1);
    if (!newSong->song.artist) { 
        free(newSong->song.name);
        free(newSong);
        return FAILURE;
    }
    strcpy(newSong->song.artist, artist);

    newSong->song.genre = genre;
    newSong->next = NULL;

    if (!playlist->head) { 
        playlist->head = newSong;
    } else {
        SongNode *curr = playlist->head;
        while (curr->next) { 
            curr = curr->next;
        }  
        curr->next = newSong;
    }
    return SUCCESS;
}


/** remove_song_from_playlist
 *
 * If a song exactly matches an existing song in playlist (all fields are 
 * equivalent), remove that song from playlist. Exit if the inputs are invalid.
 *
 * @param song the song to remove from playlist
 * @param playlist the playlist to remove song from
 */
void remove_song_from_playlist(Song *song, Playlist *playlist) {
    if (!song || !playlist) {
        return;
    }
    SongNode *curr = playlist->head;
    SongNode *prev = NULL; 
    while (curr) {
        int compared1 = strcmp(curr->song.name, song->name);
        int compared2 = strcmp(curr->song.artist, song->artist);
        int compared3 = (curr->song.genre == song->genre);
        if (compared1 == 0 && compared2 == 0 && compared3 == 1) {
            if (!prev) {
                playlist->head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr->song.name);
            free(curr->song.artist);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/** playlist_contains_song
 *
 * If playlist contains a song song, return SUCCESS, otherwise returns FAILURE.
 *
 * @param song the song to search for
 * @param playlist the playlist to check if it contains the specified song
 *
 * @return SUCCESS if playlist contains song, FAILURE otherwise or if the inputs
 * are invalid
 */
int playlist_contains_song(Song *song, Playlist *playlist) {
    if (!song || !playlist) {
        return FAILURE;
    }
    SongNode *curr = playlist->head;
    while (curr) {
        int compared1 = strcmp(curr->song.name, song->name);
        int compared2 = strcmp(curr->song.artist, song->artist);
        int compared3 = (curr->song.genre == song->genre);
        if (compared1 == 0 && compared2 == 0 && compared3 == 1) {
            return SUCCESS;
        }
        curr = curr->next;
    }
    return FAILURE;
}

/** QUEUE OPERATIONS */

/** queue_song
 *
 * Adds song to the end of the queue linked list. Because global variables are
 * zero-allocated, queue will be instantiated as NULL, so check for that case.
 *
 * @param song the song to add to the queue
 *
 * @return FAILURE upon malloc failure or invalid inputs and SUCCESS otherwise
 */
int queue_song(Song *song) {
    if (!song) {
        return FAILURE;
    }
    SongNode *newSong = malloc(sizeof(SongNode));
    if (!newSong) {
        return FAILURE;
    }

    newSong->song.name = malloc(strlen(song->name) + 1);
    if (!newSong->song.name) { 
        free(newSong);
        return FAILURE;
    }
    strcpy(newSong->song.name, song->name);

    newSong->song.artist = malloc(strlen(song->artist) + 1);
    if (!newSong->song.artist) { 
        free(newSong->song.name);
        free(newSong);
        return FAILURE;
    }
    strcpy(newSong->song.artist, song->artist);
    newSong->song.genre = song->genre;
    newSong->next = NULL;

    if (!queue) {
        queue = newSong;
    } else { 
        SongNode *curr = queue;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newSong;
    }
    return SUCCESS;
}

/** queue_playlist
 *
 * If a playlist with name exists in playlists, add all songs from that
 * playlist to the end of the queue linked list. Because global variables are
 * zero-allocated, queue will be instantiated as NULL, so check for that case.
 *
 * Consider using queue_song as a helper function for this function.
 *
 * @param name the name of the playlist to queue
 *
 * @return FAILURE upon malloc failure, if the playlist is not found, or
 * invalid inputs and SUCCESS otherwise
 */
int queue_playlist(char *name) {
    if (!name || !playlists) {
        return FAILURE;
    }
    Playlist *target = NULL; 
    for (int i = 0; i < num_playlists; i++) {
        int compared = strcmp(playlists[i]->name, name);
        if (compared ==0) {
            target = playlists[i];
        }
    }
    if (!target) {
        return FAILURE;
    } 
    SongNode *curr = target->head;
    while (curr) {
        if (queue_song(&curr->song) == FAILURE) {
            return FAILURE;
        }
        curr = curr->next;
    }
    return SUCCESS;
}

/** play_next
 *
 * Removes the first song from the queue linked list, if it exists. Exit
 * if the queue is empty.
 */
void play_next(void) {
    if (!queue) {
        return; 
    }
    SongNode *curr = queue->next;
    free(queue->song.artist);
    free(queue->song.name);
    free(queue);
    queue = curr; 
}

/** BULK PLAYLIST OPERATIONS */

/** create_playlist_of_genre
 *
 * Creates a new playlist at the end of the playlists array list containing all
 * songs of genre genre collected from all other playlists, avoiding repeats if
 * the new playlist contain the same song. If adding a song to the new playlist
 * fails, do not free the rest of the playlist but immediately return FAILURE.
 * 
 * Consider using create_playlist, add_song_to_playlist, and
 * playlist_contains_song as helper functions for this function.
 *
 * @param name the name of the new playlist to create
 * @param genre the genre of the songs to be copied to the new playlist from all
 * other playlists
 *
 * @return FAILURE upon failure of helper methods or invalid inputs and SUCCESS
 * otherwise
 */
int create_playlist_of_genre(char *name, Genre genre) {
    if (!name) {
        return FAILURE;
    }
    if (create_playlist(name) == FAILURE) {
        return FAILURE;
    }
    Playlist *new_PL = playlists[num_playlists - 1];
    for (int i = 0; i < num_playlists - 1; i++) {
        SongNode *curr = playlists[i]->head;
        while (curr) {
            if (curr->song.genre == genre) {
                if (playlist_contains_song(&curr->song, new_PL) == FAILURE) {
                    if (add_song_to_playlist(curr->song.name, curr->song.artist, curr->song.genre, new_PL) == FAILURE) {
                        return FAILURE;
                    }
                }
            }
            curr = curr->next;
        }
    }
    return SUCCESS;
}

/** copy_songs_from_artist
 *
 * Copies all songs by the artist artist from the src playlist to the dest
 * playlist, avoiding repeats if dest already contains songs that we would be
 * copying from src. If adding a song to dest fails, do not free the rest of
 * the playlist but immediately return FAILURE.
 * 
 * Consider using add_song_to_playlist and playlist_contains_song as helper
 * functions for this function.
 *
 * @param src the source playlist to copy songs from
 * @param dest the destination playlist to copy songs to
 * @param artist the artist whose songs to copy
  *
 * @return FAILURE upon failure of helper methods or invalid inputs and SUCCESS
 * otherwise
 */
int copy_songs_from_artist(Playlist *src, Playlist *dest, char *artist) {
    if (!src || !dest || !artist) {
        return FAILURE;
    }
    // Playlist *curr = NULL;
    // for (int i = 0; i < num_playlists; i++) {
    //     int compared = strcmp(src->name, playlists[i]->name);
    //     if (compared == 0) {
    //         curr = playlists[i];
    //         break;
    //     }
    // }
    // if (!curr) {
    //     return FAILURE;
    // }
    SongNode *new_curr = src->head;
    while (new_curr) {
        int compared = strcmp(new_curr->song.artist, artist);
        if (compared == 0) {
            if (playlist_contains_song(&new_curr->song, dest) == FAILURE) {
                if (add_song_to_playlist(new_curr->song.name, new_curr->song.artist, new_curr->song.genre, dest) == FAILURE) {
                    return FAILURE;
                }
            }
        }
        new_curr = new_curr->next;
    }
    return SUCCESS;
}

/** DESTROYING */

/** destroy_playlist
 *
 * Destroys the playlist at index index of playlists, making sure that all
 * remaining playlists in playlists are contiguous by shifting all playlists
 * after the removed playlist left by one. Exit if the input is invalid.
 *
 * @param index the index of the playlist to destroy
 */
void destroy_playlist(int index) {
    if (index < 0 || index >= num_playlists || !playlists) {
        return;
    }
    SongNode *curr = playlists[index]->head;
    while (curr) {
        SongNode *tmp = curr;
        curr = curr->next;
        free(tmp->song.artist);
        free(tmp->song.name);
        free(tmp);
    }
    free(playlists[index]->name);
    free(playlists[index]);
    
    for (int i = index; i < num_playlists - 1; i++) {
        playlists[i] = playlists[i+1];
    }
    playlists[--num_playlists] = NULL;
    return;
}

/** destroy_all_playlists
 *
 * Destroys all playlists in playlists, then frees the backing array of
 * playlists. Exit if playlists is null.
 * 
 * Consider using destroy_playlist as a helper function for this function.
 */
void destroy_all_playlists(void) {
    if (!playlists) {
        return;
    }
    for (int i = num_playlists - 1; i >= 0; i--) {
        destroy_playlist(i);
    }
    free(playlists);
    playlists = NULL;
    num_playlists = 0;
    playlists_capacity = 0;
    return; 
}

/** destroy_queue
 *
 * Frees all songs in queue. Exit if queue is empty.
 */
void destroy_queue(void) {
    if (!queue) {
        return;
    }
    SongNode *curr = queue;
    while (curr) {
        SongNode *tmp = curr;
        curr = curr->next;
        free(tmp->song.artist);
        free(tmp->song.name);
        free(tmp);
    }
    queue = NULL;
    return;
}
