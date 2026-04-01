#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"
#include "utils.h"

Test(queue_song, null_song) {
	cr_assert(
		eq(type(Result), queue_song(NULL), FAILURE),
		"Should return FAILURE when NULL song passed"
	);
}

Test(queue_song, malloc_failure) {
	bytes_until_fail = sizeof(SongNode) + 7;

	Song song = {.artist = "123", .name = "456", .genre = COUNTRY};
	cr_assert(eq(type(Result), queue_song(&song), FAILURE), "malloc failing should result in FAILURE");
}

Test(queue_song, no_queue) {
	Song song = {
		.name = "song",
		.artist = "artist",
		.genre = COUNTRY
	};

	cr_assert(
		eq(type(Result), queue_song(&song), SUCCESS),
		"Should return SUCCESS when song is queued"
	);

	cr_assert(not(zero(ptr, queue)), "Queue should not be NULL when a song is queued");
	cr_assert(eq(str, queue->song.name, "song"), "Song name should be set");
	cr_assert(eq(str, queue->song.artist, "artist"), "Song artist should be set");
	cr_assert(eq(type(Genre), queue->song.genre, COUNTRY), "Song genre should be set");

	free_sn(queue);
}

Test(queue_song, to_end) {
	Song song = {
		.name = "song",
		.artist = "artist",
		.genre = COUNTRY
	};

	Song song2 = {
		.name = "song2",
		.artist = "artist2",
		.genre = POP
	};

	cr_assert(
		eq(type(Result), queue_song(&song), SUCCESS),
		"Should return SUCCESS when song is queued"
	);

	cr_assert(
		eq(type(Result), queue_song(&song2), SUCCESS),
		"Should return SUCCESS when song is queued"
	);

	cr_assert(eq(str, queue->next->song.name, "song2"), "Second song name should be song2");
	cr_assert(eq(str, queue->next->song.artist, "artist2"), "Second song artist should be artist2");
	cr_assert(eq(type(Genre), queue->next->song.genre, POP), "Second song genre should be Pop");

	free_sn(queue);
}