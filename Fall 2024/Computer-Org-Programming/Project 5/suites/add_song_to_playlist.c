#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"
#include "utils.h"


Test(add_song_to_playlist, null_name) {
	Playlist playlist;
	cr_assert(
		eq(type(Result), add_song_to_playlist(NULL, "artist", COUNTRY, &playlist), FAILURE),
		"Should return FAILURE when NULL name is passed"
	);
}

Test(add_song_to_playlist, null_artist) {
	Playlist playlist;
	cr_assert(
		eq(type(Result), add_song_to_playlist("song", NULL, COUNTRY, &playlist), FAILURE),
		"Should return FAILURE when NULL artist is passed"
	);
}

Test(add_song_to_playlist, null_playlist) {
	cr_assert(
		eq(type(Result), add_song_to_playlist("The Home Depot Beat", "The Home Depot", COUNTRY, NULL), FAILURE),
		"Should return FAILURE when NULL playlist is passed"
	);
}

Test(add_song_to_playlist, malloc_failure) {
	bytes_until_fail = sizeof(SongNode) + 9;
	Playlist playlist = {.head = NULL, .name = "asdf"};

	cr_assert(
		eq(type(Result), add_song_to_playlist("Apple", "Charli xcx", POP, &playlist), FAILURE),
		"malloc failing should result in FAILURE"
	);

	cr_assert(eq(ptr, playlist.head, NULL), "Playlist should be unmodified if malloc fails");
}

Test(add_song_to_playlist, add_to_empty) {
	Playlist playlist = {
		.name = "cool playlist",
		.head = NULL,
	};
	Song song = {
		.name = "Life is a Highway",
		.artist = "Rascal Flatts",
		.genre = COUNTRY
	};
	cr_assert(
		eq(type(Result), add_song_to_playlist(song.name, song.artist, song.genre, &playlist), SUCCESS),
		"Should return SUCCESS when adding to empty playlist"
	);

	cr_assert(not(zero(ptr, playlist.head)), "Head should not be NULL");
	cr_assert(zero(ptr, playlist.head->next), "Head Next should be NULL");
	cr_assert(eq(str, playlist.head->song.artist, song.artist), "Artist name should be \"%s\"", song.artist);
	cr_assert(eq(str, playlist.head->song.name, song.name), "Song name should be \"%s\"", song.name);
	cr_assert(eq(type(Genre), playlist.head->song.genre, song.genre), "Genre should be %s", cr_user_Genre_tostr(&song.genre));

	free_sn(playlist.head);
}

Test(add_song_to_playlist, add_last) {
	Playlist playlist = {
		.name = "cooler playlist",
		.head = NULL,
	};

	cr_assert(
		eq(type(Result), add_song_to_playlist("Life is a Highway", "Rascal Flatts", COUNTRY, &playlist), SUCCESS),
		"Should return SUCCESS when adding to empty playlist"
	);

	Song song = {
		.name = "Chop Suey!",
		.artist = "System Of A Down",
		.genre = METAL
	};
	cr_assert(
		eq(type(Result), add_song_to_playlist(song.name, song.artist, song.genre, &playlist), SUCCESS),
		"Should return SUCCESS when adding to playlist"
	);

	cr_assert(not(zero(ptr, playlist.head)), "Head should not be NULL");
	cr_assert(not(zero(ptr, playlist.head->next)), "Last node should not be NULL");
	cr_assert(zero(ptr, playlist.head->next->next), "Last Node's Next should be NULL");

	cr_assert(eq(str, playlist.head->next->song.artist, song.artist), "Artist name should be \"%s\"", song.artist);
	cr_assert(eq(str, playlist.head->next->song.name, song.name), "Song name should be \"%s\"", song.name);
	cr_assert(eq(type(Genre), playlist.head->next->song.genre, song.genre), "Genre should be %s", cr_user_Genre_tostr(&song.genre));

	free_sn(playlist.head);
}

Test(add_song_to_playlist, duplicate) {
	Playlist playlist = {
		.head = NULL,
		.name = "playlist"
	};

	Song song = {
		.artist = "artist",
		.name = "song",
		.genre = POP
	};

	SongNode node = { .next = NULL, .song = song };

	playlist.head = &node;

	cr_assert(eq(type(Result), add_song_to_playlist("song", "artist", POP, &playlist), FAILURE),
		"Adding a song that already exists should result in FAILURE"
	);

	cr_assert(zero(ptr, playlist.head->next), "Playlist should not have been modified");
}