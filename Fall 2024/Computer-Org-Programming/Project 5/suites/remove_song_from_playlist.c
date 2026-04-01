#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "global.h"
#include "utils.h"

Test(remove_song_from_playlist, null_name) {
	Playlist playlist;
	remove_song_from_playlist(NULL, &playlist);
}

Test(remove_song_from_playlist, null_playlist) {
	Song song;
	remove_song_from_playlist(&song, NULL);
}

Test(remove_song_from_playlist, remove_first) {
	Playlist playlist;
	playlist.name = "Playlist";
	playlist.head = NULL;

	add_song_to_playlist("CHIHIRO", "Billie Eilish", POP, &playlist);
	add_song_to_playlist("HOT TO GO!", "Chappell Roan", POP, &playlist);
	add_song_to_playlist("Training Season", "Dua Lipa", POP, &playlist);
	add_song_to_playlist("Espresso", "Sabrina Carpenter", POP, &playlist);

	// Test remove:
	Song song = {
		.name = "CHIHIRO",
		.artist = "Billie Eilish",
		.genre = POP
	};
	remove_song_from_playlist(&song, &playlist);
	cr_assert(not(zero(ptr, playlist.head)), "Expected playlist to have at least 3 elements, got 0");
	cr_assert(eq(str, playlist.head->song.name, "HOT TO GO!"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist.head->song.artist, "Chappell Roan"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->song.genre, POP), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next)), "Expected playlist to have at least 3 elements, got 1");
	cr_assert(eq(str, playlist.head->next->song.name, "Training Season"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->song.artist, "Dua Lipa"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->song.genre, POP), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next->next)), "Expected playlist to have 3 elements, got 2");
	cr_assert(eq(str, playlist.head->next->next->song.name, "Espresso"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->next->song.artist, "Sabrina Carpenter"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->next->song.genre, POP), "Playlist's 3rd song's genre did not match expected");

	cr_assert(zero(ptr, playlist.head->next->next->next), "Expected playlist to have 3 elements, got more");

	free_sn(playlist.head);
}

Test(remove_song_from_playlist, remove_middle) {
	Playlist playlist;
	playlist.name = "Playlist";
	playlist.head = NULL;

	add_song_to_playlist("CHIHIRO", "Billie Eilish", POP, &playlist);
	add_song_to_playlist("HOT TO GO!", "Chappell Roan", POP, &playlist);
	add_song_to_playlist("Training Season", "Dua Lipa", POP, &playlist);
	add_song_to_playlist("Espresso", "Sabrina Carpenter", POP, &playlist);

	// Test remove:
	Song song = {
		.name = "HOT TO GO!",
		.artist = "Chappell Roan",
		.genre = POP
	};
	remove_song_from_playlist(&song, &playlist);
	cr_assert(not(zero(ptr, playlist.head)), "Expected playlist to have at least 3 elements, got 0");
	cr_assert(eq(str, playlist.head->song.name, "CHIHIRO"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist.head->song.artist, "Billie Eilish"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->song.genre, POP), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next)), "Expected playlist to have at least 3 elements, got 1");
	cr_assert(eq(str, playlist.head->next->song.name, "Training Season"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->song.artist, "Dua Lipa"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->song.genre, POP), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next->next)), "Expected playlist to have 3 elements, got 2");
	cr_assert(eq(str, playlist.head->next->next->song.name, "Espresso"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->next->song.artist, "Sabrina Carpenter"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->next->song.genre, POP), "Playlist's 3rd song's genre did not match expected");

	cr_assert(zero(ptr, playlist.head->next->next->next), "Expected playlist to have 3 elements, got more");
	
	free_sn(playlist.head);
}

Test(remove_song_from_playlist, remove_last) {
	Playlist playlist;
	playlist.name = "Playlist";
	playlist.head = NULL;

	add_song_to_playlist("CHIHIRO", "Billie Eilish", POP, &playlist);
	add_song_to_playlist("HOT TO GO!", "Chappell Roan", POP, &playlist);
	add_song_to_playlist("Training Season", "Dua Lipa", POP, &playlist);
	add_song_to_playlist("Espresso", "Sabrina Carpenter", POP, &playlist);

	// Test remove:
	Song song = {
		.name = "Espresso",
		.artist = "Sabrina Carpenter",
		.genre = POP
	};
	remove_song_from_playlist(&song, &playlist);
	cr_assert(not(zero(ptr, playlist.head)), "Expected playlist to have at least 3 elements, got 0");
	cr_assert(eq(str, playlist.head->song.name, "CHIHIRO"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist.head->song.artist, "Billie Eilish"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->song.genre, POP), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next)), "Expected playlist to have at least 3 elements, got 1");
	cr_assert(eq(str, playlist.head->next->song.name, "HOT TO GO!"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->song.artist, "Chappell Roan"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->song.genre, POP), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next->next)), "Expected playlist to have 3 elements, got 2");
	cr_assert(eq(str, playlist.head->next->next->song.name, "Training Season"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->next->song.artist, "Dua Lipa"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->next->song.genre, POP), "Playlist's 3rd song's genre did not match expected");

	cr_assert(zero(ptr, playlist.head->next->next->next), "Expected playlist to have 3 elements, got more");

	free_sn(playlist.head);
}

Test(remove_song_from_playlist, no_change) {
	Playlist playlist;
	playlist.name = "Playlist";
	playlist.head = NULL;

	add_song_to_playlist("CHIHIRO", "Billie Eilish", POP, &playlist);
	add_song_to_playlist("HOT TO GO!", "Chappell Roan", POP, &playlist);
	add_song_to_playlist("Training Season", "Dua Lipa", POP, &playlist);
	add_song_to_playlist("Espresso", "Sabrina Carpenter", POP, &playlist);

	// Test remove:
	Song song = {
		.name = "Girl, so confusing",
		.artist = "Charli xcx",
		.genre = POP
	};
	remove_song_from_playlist(&song, &playlist);
	cr_assert(not(zero(ptr, playlist.head)), "Expected playlist to have at least 4 elements, got 0");
	cr_assert(eq(str, playlist.head->song.name, "CHIHIRO"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist.head->song.artist, "Billie Eilish"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->song.genre, POP), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next)), "Expected playlist to have at least 4 elements, got 1");
	cr_assert(eq(str, playlist.head->next->song.name, "HOT TO GO!"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->song.artist, "Chappell Roan"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->song.genre, POP), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next->next)), "Expected playlist to have 4 elements, got 2");
	cr_assert(eq(str, playlist.head->next->next->song.name, "Training Season"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->next->song.artist, "Dua Lipa"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->next->song.genre, POP), "Playlist's 3rd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist.head->next->next->next)), "Expected playlist to have 4 elements, got 3");
	cr_assert(eq(str, playlist.head->next->next->next->song.name, "Espresso"), "Playlist's 4th song's name did not match expected");
	cr_assert(eq(str, playlist.head->next->next->next->song.artist, "Sabrina Carpenter"), "Playlist's 4th song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist.head->next->next->next->song.genre, POP), "Playlist's 4th song's genre did not match expected");

	cr_assert(zero(ptr, playlist.head->next->next->next->next), "Expected playlist to have 4 elements, got more");

	free_sn(playlist.head);
}