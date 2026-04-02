#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"

Test(create_playlist, null_name) {
	cr_assert(
		eq(type(Result), create_playlist(NULL), FAILURE),
		"Should return FAILURE when NULL name passed"
	);
}

Test(create_playlist, init_playlists_not_called) {
	playlists = NULL;
	playlists_capacity = 1;

	cr_assert(
		eq(type(Result), create_playlist("abc"), FAILURE),
		"Should return FAILURE when playlists not initialized"
	);

	playlists = (Playlist **)0x1; // set to non-NULL value
	playlists_capacity = 0;

	cr_assert(
		eq(type(Result), create_playlist("def"), FAILURE),
		"Should return FAILURE when playlists capacity is 0"
	);
}

Test(create_playlist, resize_failure) {
	init_playlists(2);
	bytes_until_fail = 3 * sizeof(Playlist) + 11;
	create_playlist("abc");
	create_playlist("def");
	cr_assert(eq(type(Result), create_playlist("lol"), FAILURE), "malloc failing should result in FAILURE");

	destroy_all_playlists();
}

Test(create_playlist, malloc_failure) {
	init_playlists(1);
	bytes_until_fail = sizeof(Playlist) + 3;
	cr_assert(eq(type(Result), create_playlist("lol"), FAILURE), "malloc failing should result in FAILURE");
	cr_assert(eq(int, num_playlists, 0), "num_playlists should not be modified if playlist could not be created");

	destroy_all_playlists();
}

Test(create_playlist, success) {
	char *playlist_names[] = { "Dark Mix", "Floaty Mix", "Rizz Mix", "Sleepy Weepy Mix" };

	init_playlists(1);

	cr_assert(eq(type(Result), create_playlist(playlist_names[0]), SUCCESS), "Should return SUCCESS when valid name passed");
	cr_assert(eq(i32, num_playlists, 1), "Number of playlists should be 1");
	cr_assert(eq(str, playlists[0]->name, playlist_names[0]), "Playlist name should be set on creation");
	cr_assert(zero(ptr, playlists[0]->head), "Playlist should be empty on creation");

	cr_assert(eq(type(Result), create_playlist(playlist_names[1]), SUCCESS), "Should return SUCCESS when valid name passed");
	cr_assert(eq(i32, num_playlists, 2), "Number of playlists should be 2");
	cr_assert(eq(str, playlists[1]->name, playlist_names[1]), "Playlist name should be set on creation");
	cr_assert(zero(ptr, playlists[1]->head), "Playlist should be empty on creation");

	cr_assert(eq(type(Result), create_playlist(playlist_names[2]), SUCCESS), "Should return SUCCESS when valid name passed");
	cr_assert(eq(i32, num_playlists, 3), "Number of playlists should be 3");
	cr_assert(eq(str, playlists[2]->name, playlist_names[2]), "Playlist name should be set on creation");
	cr_assert(zero(ptr, playlists[2]->head), "Playlist should be empty on creation");

	cr_assert(eq(type(Result), create_playlist(playlist_names[3]), SUCCESS), "Should return SUCCESS when valid name passed");
	cr_assert(eq(i32, num_playlists, 4), "Number of playlists should be 4");
	cr_assert(eq(str, playlists[3]->name, playlist_names[3]), "Playlist name should be set on creation");
	cr_assert(zero(ptr, playlists[3]->head), "Playlist should be empty on creation");

	destroy_all_playlists();
}