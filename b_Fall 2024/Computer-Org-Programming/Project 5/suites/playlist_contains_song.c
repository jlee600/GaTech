#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "global.h"
#include "utils.h"

Test(playlist_contains_song, null_playlist) {
	Song song;
	cr_assert(
		eq(type(Result), playlist_contains_song(&song, NULL), FAILURE),
		"Should return FAILURE when NULL playlist passed"
	);
}

Test(playlist_contains_song, null_song) {
	Playlist playlist;
	cr_assert(
		eq(type(Result), playlist_contains_song(NULL, &playlist), FAILURE),
		"Should return FAILURE when NULL name passed"
	);
}

Test(playlist_contains_song, empty_playlist) {
	Playlist playlist;
	playlist.name = "Nothing";
	playlist.head = NULL;

	// empty name, artist
	Song blank_song = {
		.name = "",
		.artist = "",
		.genre = LOFI,
	};
	cr_assert(
		eq(type(Result), playlist_contains_song(&blank_song, &playlist), FAILURE),
		"Song does not exist on playlist"
	);

	// names and artists with the name NULL
	Song fake_null_song = {
		.name = "NULL",
		.artist = "NULL",
		.genre = COUNTRY
	};
	cr_assert(
		eq(type(Result), playlist_contains_song(&fake_null_song, &playlist), FAILURE),
		"Song does not exist on playlist"
	);

	// normal test
	Song song = {
		.name = "Read Your Mind",
		.artist = "Sabrina Carpenter",
		.genre = POP
	};
	cr_assert(
		eq(type(Result), playlist_contains_song(&song, &playlist), FAILURE),
		"Song does not exist on playlist"
	);
}

Test(playlist_contains_song, first) {
	Playlist playlist;
	playlist.name = "Playlist";
	playlist.head = NULL;

	// TODO: don't use the student's function to set up the case
	// isolate the tested behavior better with helper function
	// this will be done in a later revision
	add_song_to_playlist("serotonina", "HUMBE", LOFI, &playlist);
	add_song_to_playlist("Diosa", "Kali Uchis", LOFI, &playlist);
	add_song_to_playlist("The Other Side of Paradise", "Glass Animals", POP, &playlist);

	// Check for song:
	Song song = {
		.name = "serotonina",
		.artist = "HUMBE",
		.genre = LOFI
	};
	cr_assert(
		eq(type(Result), playlist_contains_song(&song, &playlist), SUCCESS),
		"Song (%s - %s) should have been present on playlist",
		song.artist, song.name
	);

	free_sn(playlist.head);
}

Test(playlist_contains_song, last) {
	Playlist playlist;
	playlist.name = "Playlist";
	playlist.head = NULL;

	add_song_to_playlist("serotonina", "HUMBE", LOFI, &playlist);
	add_song_to_playlist("Diosa", "Kali Uchis", LOFI, &playlist);
	add_song_to_playlist("The Other Side of Paradise", "Glass Animals", POP, &playlist);

	// Check for song:
	Song song = {
		.name = "The Other Side of Paradise",
		.artist = "Glass Animals",
		.genre = POP
	};
	cr_assert(
		eq(type(Result), playlist_contains_song(&song, &playlist), SUCCESS),
		"Song (%s - %s) should have been present on playlist",
		song.artist, song.name
	);

	free_sn(playlist.head);
}

Test(playlist_contains_song, not_found) {
	Playlist playlist;
	playlist.name = "Playlist";
	playlist.head = NULL;

	add_song_to_playlist("serotonina", "HUMBE", LOFI, &playlist);
	add_song_to_playlist("Diosa", "Kali Uchis", LOFI, &playlist);
	add_song_to_playlist("The Other Side of Paradise", "Glass Animals", POP, &playlist);

	// Check for song:
	Song song = {
		.name = "BLUE",
		.artist = "Billie Eilish",
		.genre = LOFI
	};
	cr_assert(
		eq(type(Result), playlist_contains_song(&song, &playlist), FAILURE),
		"Song (%s - %s) should not have been present on playlist",
		song.artist, song.name
	);

	free_sn(playlist.head);
}