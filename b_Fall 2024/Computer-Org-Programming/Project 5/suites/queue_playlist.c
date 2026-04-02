#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"
#include "utils.h"

Test(queue_playlist, null_name) {
	cr_assert(
		eq(type(Result), queue_playlist(NULL), FAILURE),
		"Should return FAILURE when NULL name passed"
	);
}

Test(queue_playlist, does_not_exist) {
	cr_assert(
		eq(type(Result), queue_playlist("playlist"), FAILURE),
		"Should return FAILURE when playlist does not exist"
	);
}

Test(queue_playlist, queue_failure) {
	init_playlists(1);
	bytes_until_fail = sizeof(SongNode) + 7;

	SongNode node = {.song = {.artist = "123", .name = "456", .genre = COUNTRY}, .next = NULL};
	Playlist playlist = {.head = &node, .name = "playlist"};
	playlists[num_playlists++] = &playlist;

	cr_assert(eq(type(Result), queue_playlist("playlist"), FAILURE), "malloc failing should result in FAILURE");

	free(playlists);
}

Test(queue_playlist, empty_playlist) {
	Playlist playlist = {
		.name = "playlist",
		.head = NULL
	};
	
	Playlist* stack_backing_arr[5] = { 0 };
	playlists = stack_backing_arr;
	playlists[0] = &playlist;
	num_playlists++;

	cr_assert(
		eq(type(Result), queue_playlist("playlist"), SUCCESS),
		"Should return SUCCESS when playlist is queued"
	);

	// make sure all songs are queued 
	cr_assert(zero(ptr, queue), "Queue should be NULL when playlist is empty");
}

Test(queue_playlist, success) {
	Playlist playlist = {
		.name = "playlist",
		.head = NULL
	};

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

	SongNode song_node_2 = {
		.song = song2,
		.next = NULL
	};

	SongNode song_node = {
		.song = song,
		.next = &song_node_2
	};

	playlist.head = &song_node;

	Playlist* stack_backing_arr[5] = { 0 };
	playlists = stack_backing_arr;
	playlists[0] = &playlist;
	num_playlists++;

	cr_assert(
		eq(type(Result), queue_playlist("playlist"), SUCCESS),
		"Should return SUCCESS when playlist is queued"
	);

	// make sure all songs are queued 
	cr_assert(not(zero(ptr, queue)), "Queue should not be NULL when a playlist is queued");
	cr_assert(eq(str, queue->song.name, "song"), "First song name should be set");
	cr_assert(eq(str, queue->song.artist, "artist"), "First song artist should be set");
	cr_assert(eq(type(Genre), queue->song.genre, COUNTRY), "First song genre should be set");

	cr_assert(not(zero(ptr, queue->next)), "Second song should be queued in playlist");
	cr_assert(eq(str, queue->next->song.name, "song2"), "Second song name should be set");
	cr_assert(eq(str, queue->next->song.artist, "artist2"), "Second song artist should be set");
	cr_assert(eq(type(Genre), queue->next->song.genre, POP), "Second song genre should be set");

	free_sn(queue);
}