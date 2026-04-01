#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"

Test(init_playlists, negative_capacity) {
	cr_assert(
		eq(type(Result), init_playlists(-1), FAILURE),
		"Should return FAILURE when negative capacity passed"
	);
}

Test(init_playlists, zero_capacity) {
	cr_assert(
		eq(type(Result), init_playlists(0), FAILURE),
		"Should return FAILURE when zero capacity passed"
	);
}

Test(init_playlists, malloc_failure) {
	bytes_until_fail = sizeof(Playlist*) - 1;
	cr_assert(eq(type(Result), init_playlists(1), FAILURE), "malloc failing should result in FAILURE");
	cr_assert(eq(int, playlists_capacity, 0), "capacity should remain unmodified if failed");
}

Test(init_playlists, success) {
	cr_assert(
		eq(type(Result), init_playlists(1), SUCCESS),
		"Should return SUCCESS when valid capacity passed"
	);

	cr_assert(eq(i32, playlists_capacity, 1), "Capacity should be 1");

	destroy_all_playlists();
}