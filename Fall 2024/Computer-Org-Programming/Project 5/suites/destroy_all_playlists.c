#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"

Test(destroy_all_playlists, null_playlists) {
	playlists = NULL;
	destroy_all_playlists();
}

Test(destroy_all_playlists, success) {
	init_playlists(5);
	create_playlist("i am a playlist");
	create_playlist("i am another playlist");
	create_playlist("i am also another playlist");
	create_playlist("asdfghjkl");
	add_song_to_playlist("someone should", "really update this", POP, playlists[0]);
	add_song_to_playlist("with real songs", "at some point", POP, playlists[1]);

	// Test
	destroy_all_playlists();
	cr_assert(eq(int, num_playlists, 0), "Expected no playlists in the global playlist array");
}