#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"
#include "utils.h"

Test(destroy_playlist, negative_index) {
	char *playlist_names[] = { "Delulu Mix", "Bed Rotting Mix", "Clowncore Mix", "Vampiric Mix" };
	init_playlists(4);
	for (int i = 0; i < 4; i++) {
		create_playlist(playlist_names[i]);
	}
	cr_assert(eq(int, num_playlists, 4), "should have intialized playlists successfully");

	// Test
	destroy_playlist(-1);
	cr_assert(eq(int, num_playlists, 4), "should not have destroyed any playlist");
}

Test(destroy_playlist, index_too_high) {
	char *playlist_names[] = { "Delulu Mix", "Bed Rotting Mix", "Clowncore Mix", "Vampiric Mix" };
	init_playlists(4);
	for (int i = 0; i < 4; i++) {
		create_playlist(playlist_names[i]);
	}
	cr_assert(eq(int, num_playlists, 4), "should have intialized playlists successfully");

	// Test
	destroy_playlist(10000);
	cr_assert(eq(int, num_playlists, 4), "should not have destroyed any playlist");
}

Test(destroy_playlist, success) {
	char *playlist_names[] = { "Delulu Mix", "Bed Rotting Mix", "Clowncore Mix", "Vampiric Mix" };
	
	init_playlists(4);
	for (int i = 0; i < 4; i++) {
		create_playlist(playlist_names[i]);
	}
	cr_assert(eq(int, num_playlists, 4), "should have intialized playlists successfully");
	add_song_to_playlist("abc", "def", ROCK, playlists[0]);
	add_song_to_playlist("ghi", "jkl", POP, playlists[1]);
	add_song_to_playlist("mno", "pqr", LOFI, playlists[2]);
	add_song_to_playlist("stu", "vwx", COUNTRY, playlists[3]);

	// Test:
	destroy_playlist(1);
	cr_assert(eq(int, num_playlists, 3), "num_playlists should have been decremented after destroy_playlist call");
	cr_assert(eq(str, playlists[0]->name, playlist_names[0]), "Expected playlist to be correct after destroy_playlist call");
	cr_assert(eq(str, playlists[1]->name, playlist_names[2]), "Expected playlist to be correct after destroy_playlist call");
	cr_assert(eq(str, playlists[2]->name, playlist_names[3]), "Expected playlist to be correct after destroy_playlist call");
	
	destroy_playlist(2);
	cr_assert(eq(int, num_playlists, 2), "num_playlists should have been decremented after destroy_playlist call");
	cr_assert(eq(str, playlists[0]->name, playlist_names[0]), "Expected playlist to be correct after destroy_playlist call");
	cr_assert(eq(str, playlists[1]->name, playlist_names[2]), "Expected playlist to be correct after destroy_playlist call");
	
	destroy_playlist(0);
	cr_assert(eq(int, num_playlists, 1), "num_playlists should have been decremented after destroy_playlist call");
	cr_assert(eq(str, playlists[0]->name, playlist_names[2]), "Expected playlist to be correct after destroy_playlist call");

	destroy_playlist(0);
	cr_assert(eq(int, num_playlists, 0), "num_playlists should have been decremented after destroy_playlist call");
}