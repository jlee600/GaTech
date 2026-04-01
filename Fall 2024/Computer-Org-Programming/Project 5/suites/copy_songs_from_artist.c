#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "global.h"
#include "utils.h"

Test(copy_songs_from_artist, null_src) {
	Playlist playlist;
	cr_assert(
		eq(type(Result), copy_songs_from_artist(NULL, &playlist, "artist"), FAILURE),
		"Should return FAILURE when NULL src is passed"
	);
}

Test(copy_songs_from_artist, null_dest) {
	Playlist playlist;
	cr_assert(
		eq(type(Result), copy_songs_from_artist(&playlist, NULL, "artist"), FAILURE),
		"Should return FAILURE when NULL dest is passed"
	);
}

Test(copy_songs_from_artist, null_artist) {
	Playlist playlist, other;
	cr_assert(
		eq(type(Result), copy_songs_from_artist(&playlist, &other, NULL), FAILURE),
		"Should return FAILURE when NULL artist is passed"
	);
}

Test(copy_songs_from_artist, malloc_failure) {
	bytes_until_fail = sizeof(SongNode) + 9;
	Playlist src, dst;
	SongNode node = {
		.next = NULL,
		.song = { .name = "Metal Pipe Falling Sound Effect", .artist = "Unnamed", .genre = METAL }
	};
	src.head = &node;
	dst.head = NULL;

	cr_assert(
		eq(type(Result), copy_songs_from_artist(&src, &dst, "Unnamed"), FAILURE),
		"malloc failing should result in FAILURE"
	);
}

Test(copy_songs_from_artist, simple) {
	Playlist pla = {
		.name = "Playlist A",
		.head = NULL
	};
	add_song_to_playlist("LUNCH", "Billie Eilish", POP, &pla);
	add_song_to_playlist("Valentine", "Laufey", LOFI, &pla);
	add_song_to_playlist("Are You Bored Yet?", "Wallows", LOFI, &pla);
	
	Playlist plb = {
		.name = "Playlist B",
		.head = NULL
	};
	add_song_to_playlist("Falling Behind", "Laufey", LOFI, &plb);
	add_song_to_playlist("Everything I Know About Love", "Laufey", LOFI, &plb);
	add_song_to_playlist("Good Luck, Babe!", "Chappell Roan", POP, &plb);

	// Test
	cr_assert(
		eq(type(Result), copy_songs_from_artist(&plb, &pla, "Laufey"), SUCCESS),
		"Songs should have been successfully copied to the new playlist"
	);
	cr_assert(not(zero(ptr, pla.head)), "Expected playlist to have at least 5 elements, got 0");
	cr_assert(eq(str, pla.head->song.name, "LUNCH"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, pla.head->song.artist, "Billie Eilish"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->song.genre, POP), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, pla.head->next)), "Expected playlist to have at least 5 elements, got 1");
	cr_assert(eq(str, pla.head->next->song.name, "Valentine"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, pla.head->next->song.artist, "Laufey"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->next->song.genre, LOFI), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, pla.head->next->next)), "Expected playlist to have 5 elements, got 2");
	cr_assert(eq(str, pla.head->next->next->song.name, "Are You Bored Yet?"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, pla.head->next->next->song.artist, "Wallows"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->next->next->song.genre, LOFI), "Playlist's 3rd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, pla.head->next->next->next)), "Expected playlist to have 5 elements, got 3");
	cr_assert(eq(str, pla.head->next->next->next->song.name, "Falling Behind"), "Playlist's 4th song's name did not match expected");
	cr_assert(eq(str, pla.head->next->next->next->song.artist, "Laufey"), "Playlist's 4th song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->next->next->next->song.genre, LOFI), "Playlist's 4th song's genre did not match expected");
	
	cr_assert(not(zero(ptr, pla.head->next->next->next->next)), "Expected playlist to have 5 elements, got 4");
	cr_assert(eq(str, pla.head->next->next->next->next->song.name, "Everything I Know About Love"), "Playlist's 5th song's name did not match expected");
	cr_assert(eq(str, pla.head->next->next->next->next->song.artist, "Laufey"), "Playlist's 5th song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->next->next->next->next->song.genre, LOFI), "Playlist's 5th song's genre did not match expected");

	cr_assert(zero(ptr, pla.head->next->next->next->next->next), "Expected playlist to have 5 elements, got more");

	free_sn(pla.head);
	free_sn(plb.head);
}

Test(copy_songs_from_artist, duplicates) {
	Playlist pla = {
		.name = "Playlist A",
		.head = NULL
	};
	add_song_to_playlist("LUNCH", "Billie Eilish", POP, &pla);
	add_song_to_playlist("Valentine", "Laufey", LOFI, &pla);
	add_song_to_playlist("Are You Bored Yet?", "Wallows", LOFI, &pla);
	
	Playlist plb = {
		.name = "Playlist B",
		.head = NULL
	};
	add_song_to_playlist("Falling Behind", "Laufey", LOFI, &plb);
	add_song_to_playlist("Valentine", "Laufey", LOFI, &plb);
	add_song_to_playlist("Good Luck, Babe!", "Chappell Roan", POP, &plb);

	// Test
	cr_assert(
		eq(type(Result), copy_songs_from_artist(&plb, &pla, "Laufey"), SUCCESS),
		"Songs should have been successfully copied to the new playlist"
	);
	cr_assert(not(zero(ptr, pla.head)), "Expected playlist to have at least 4 elements, got 0");
	cr_assert(eq(str, pla.head->song.name, "LUNCH"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, pla.head->song.artist, "Billie Eilish"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->song.genre, POP), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, pla.head->next)), "Expected playlist to have at least 4 elements, got 1");
	cr_assert(eq(str, pla.head->next->song.name, "Valentine"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, pla.head->next->song.artist, "Laufey"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->next->song.genre, LOFI), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, pla.head->next->next)), "Expected playlist to have 4 elements, got 2");
	cr_assert(eq(str, pla.head->next->next->song.name, "Are You Bored Yet?"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, pla.head->next->next->song.artist, "Wallows"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->next->next->song.genre, LOFI), "Playlist's 3rd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, pla.head->next->next->next)), "Expected playlist to have 4 elements, got 3");
	cr_assert(eq(str, pla.head->next->next->next->song.name, "Falling Behind"), "Playlist's 4th song's name did not match expected");
	cr_assert(eq(str, pla.head->next->next->next->song.artist, "Laufey"), "Playlist's 4th song's artist did not match expected");
	cr_assert(eq(type(Genre), pla.head->next->next->next->song.genre, LOFI), "Playlist's 4th song's genre did not match expected");
	
	cr_assert(zero(ptr, pla.head->next->next->next->next), "Expected playlist to have 4 elements, got more");

	free_sn(pla.head);
	free_sn(plb.head);

	destroy_all_playlists();
}
