#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "global.h"

void setup(void) {
	init_playlists(3);
	create_playlist("Pla");
	create_playlist("Plb");
	create_playlist("Plc");
}

void teardown(void) {
	destroy_all_playlists();
}

Test(create_playlist_of_genre, null_name) {
	cr_assert(
		eq(type(Result), create_playlist_of_genre(NULL, POP), FAILURE),
		"Should return FAILURE when null name passed"
	);
}

Test(create_playlist_of_genre, malloc_failure) {
	init_playlists(1); // TODO remove need for this line
	bytes_until_fail = sizeof(Playlist) + 3;
	cr_assert(eq(type(Result), create_playlist_of_genre("lol", POP), FAILURE), "malloc failing should result in FAILURE");
	destroy_all_playlists();

	bytes_until_fail = -1;
	init_playlists(2);
	num_playlists = 0;
	create_playlist("asdf");
	add_song_to_playlist("12345", "abc", POP, playlists[0]); // TODO remove need for this

	bytes_until_fail = sizeof(Playlist) + sizeof(SongNode) + 13;
	cr_assert(eq(type(Result), create_playlist_of_genre("lol", POP), FAILURE), "malloc failing should result in FAILURE");
	destroy_all_playlists();
}

Test(create_playlist_of_genre, simple, .init = setup, .fini = teardown) {
	Song songs_a[] = {
		{ .name = "LUNCH", .artist = "Billie Eilish", .genre = POP },
		{ .name = "Valentine", .artist = "Laufey", .genre = LOFI },
		{ .name = "Are You Bored Yet?", .artist = "Wallows", .genre = LOFI },
		{ .name = "Enter Sandman", .artist = "Metallica", .genre = METAL },
		{ .name = "Pork Soda", .artist = "Glass Animals", .genre = POP },
	};
	Song songs_b[] = {
		{ .name = "Jolene", .artist = "Dolly Parton", .genre = COUNTRY },
		{ .name = "Livin' On A Prayer", .artist = "Bon Jovi", .genre = ROCK },
		{ .name = "Tacones Rojos", .artist = "Sebastian Yatra", .genre = POP },
		{ .name = "Future Nostalgia", .artist = "Dua Lipa", .genre = POP },
		{ .name = "thank u, next", .artist = "Ariana Grande", .genre = POP },
	};
	Song songs_c[] = {
		{ .name = "I Wish you Roses", .artist = "Kali Uchis", .genre = LOFI },
		{ .name = "Take Me Home, Country Roads", .artist = "John Denver", .genre = COUNTRY },
		{ .name = "Creep", .artist = "Radiohead", .genre = ROCK },
		{ .name = "Hotel California", .artist = "Eagles", .genre = ROCK },
		{ .name = "Invisivel", .artist = "Jovem Dionisio", .genre = LOFI },
	};

	for (int i = 0; i < 5; i++) {
		add_song_to_playlist(songs_a[i].name, songs_a[i].artist, songs_a[i].genre, playlists[0]);
		add_song_to_playlist(songs_b[i].name, songs_b[i].artist, songs_b[i].genre, playlists[1]);
		add_song_to_playlist(songs_c[i].name, songs_c[i].artist, songs_c[i].genre, playlists[2]);
	}

	// Tests.
	Playlist *playlist;
	// Country
	cr_assert(
		eq(type(Result), create_playlist_of_genre("Good Country Playlist", COUNTRY), SUCCESS),
		"Expected creation of playlist to succeed"
	);
	playlist = playlists[3];
	cr_assert(not(zero(ptr, playlist->head)), "Expected playlist to have at least 2 elements, got 0");
	cr_assert(eq(str, playlist->head->song.name, "Jolene"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist->head->song.artist, "Dolly Parton"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->song.genre, COUNTRY), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next)), "Expected playlist to have at least 2 elements, got 1");
	cr_assert(eq(str, playlist->head->next->song.name, "Take Me Home, Country Roads"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->song.artist, "John Denver"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->song.genre, COUNTRY), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(zero(ptr, playlist->head->next->next), "Expected playlist to have 2 elements, got more");
	
	// Pop
	cr_assert(
		eq(type(Result), create_playlist_of_genre("Good Pop Playlist", POP), SUCCESS),
		"Expected creation of playlist to succeed"
	);
	playlist = playlists[4];
	cr_assert(not(zero(ptr, playlist->head)), "Expected playlist to have at least 5 elements, got 0");
	cr_assert(eq(str, playlist->head->song.name, "LUNCH"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist->head->song.artist, "Billie Eilish"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->song.genre, POP), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next)), "Expected playlist to have at least 5 elements, got 1");
	cr_assert(eq(str, playlist->head->next->song.name, "Pork Soda"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->song.artist, "Glass Animals"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->song.genre, POP), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next->next)), "Expected playlist to have 5 elements, got 2");
	cr_assert(eq(str, playlist->head->next->next->song.name, "Tacones Rojos"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->next->song.artist, "Sebastian Yatra"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->next->song.genre, POP), "Playlist's 3rd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next->next->next)), "Expected playlist to have 5 elements, got 3");
	cr_assert(eq(str, playlist->head->next->next->next->song.name, "Future Nostalgia"), "Playlist's 4th song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->next->next->song.artist, "Dua Lipa"), "Playlist's 4th song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->next->next->song.genre, POP), "Playlist's 4th song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next->next->next->next)), "Expected playlist to have 5 elements, got 4");
	cr_assert(eq(str, playlist->head->next->next->next->next->song.name, "thank u, next"), "Playlist's 5th song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->next->next->next->song.artist, "Ariana Grande"), "Playlist's 5th song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->next->next->next->song.genre, POP), "Playlist's 5th song's genre did not match expected");

	cr_assert(zero(ptr, playlist->head->next->next->next->next->next), "Expected playlist to have 5 elements, got more");
	
	// Rock
	cr_assert(
		eq(type(Result), create_playlist_of_genre("Good Rock Playlist", ROCK), SUCCESS),
		"Expected creation of playlist to succeed"
	);
	playlist = playlists[5];
	cr_assert(not(zero(ptr, playlist->head)), "Expected playlist to have at least 3 elements, got 0");
	cr_assert(eq(str, playlist->head->song.name, "Livin' On A Prayer"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist->head->song.artist, "Bon Jovi"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->song.genre, ROCK), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next)), "Expected playlist to have at least 3 elements, got 1");
	cr_assert(eq(str, playlist->head->next->song.name, "Creep"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->song.artist, "Radiohead"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->song.genre, ROCK), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next->next)), "Expected playlist to have 3 elements, got 2");
	cr_assert(eq(str, playlist->head->next->next->song.name, "Hotel California"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->next->song.artist, "Eagles"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->next->song.genre, ROCK), "Playlist's 3rd song's genre did not match expected");

	cr_assert(zero(ptr, playlist->head->next->next->next), "Expected playlist to have 3 elements, got more");
	
	// LoFi
	cr_assert(
		eq(type(Result), create_playlist_of_genre("Good LoFi Playlist", LOFI), SUCCESS),
		"Expected creation of playlist to succeed"
	);
	playlist = playlists[6];
	cr_assert(not(zero(ptr, playlist->head)), "Expected playlist to have at least 4 elements, got 0");
	cr_assert(eq(str, playlist->head->song.name, "Valentine"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist->head->song.artist, "Laufey"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->song.genre, LOFI), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next)), "Expected playlist to have at least 4 elements, got 1");
	cr_assert(eq(str, playlist->head->next->song.name, "Are You Bored Yet?"), "Playlist's 2nd song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->song.artist, "Wallows"), "Playlist's 2nd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->song.genre, LOFI), "Playlist's 2nd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next->next)), "Expected playlist to have 4 elements, got 2");
	cr_assert(eq(str, playlist->head->next->next->song.name, "I Wish you Roses"), "Playlist's 3rd song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->next->song.artist, "Kali Uchis"), "Playlist's 3rd song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->next->song.genre, LOFI), "Playlist's 3rd song's genre did not match expected");
	
	cr_assert(not(zero(ptr, playlist->head->next->next->next)), "Expected playlist to have 4 elements, got 3");
	cr_assert(eq(str, playlist->head->next->next->next->song.name, "Invisivel"), "Playlist's 4th song's name did not match expected");
	cr_assert(eq(str, playlist->head->next->next->next->song.artist, "Jovem Dionisio"), "Playlist's 4th song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->next->next->next->song.genre, LOFI), "Playlist's 4th song's genre did not match expected");
	
	cr_assert(zero(ptr, playlist->head->next->next->next->next), "Expected playlist to have 4 elements, got more");
	
	// Metal
	cr_assert(
		eq(type(Result), create_playlist_of_genre("Good Metal Playlist", METAL), SUCCESS),
		"Expected creation of playlist to succeed"
	);
	playlist = playlists[7];
	cr_assert(not(zero(ptr, playlist->head)), "Expected playlist to have at least 1 element, got 0");
	cr_assert(eq(str, playlist->head->song.name, "Enter Sandman"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist->head->song.artist, "Metallica"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->song.genre, METAL), "Playlist's 1st song's genre did not match expected");
	
	cr_assert(zero(ptr, playlist->head->next), "Expected playlist to have 1 element, got more");
}

Test(create_playlist_of_genre, duplicates, .init = setup, .fini = teardown) {
	// i'm a creep, i'm a weirdo
	add_song_to_playlist("Creep", "Radiohead", ROCK, playlists[0]);
	add_song_to_playlist("Enter Sandman", "Metallica", METAL, playlists[0]);

	add_song_to_playlist("Creep", "Radiohead", ROCK, playlists[1]);
	add_song_to_playlist("Jolene", "Dolly Parton", COUNTRY, playlists[1]);

	add_song_to_playlist("Creep", "Radiohead", ROCK, playlists[2]);
	add_song_to_playlist("I Wish you Roses", "Kali Uchis", LOFI, playlists[2]);

	// Test
	cr_assert(
		eq(type(Result), create_playlist_of_genre("Good Rock Playlist", ROCK), SUCCESS),
		"Expected creation of playlist to succeed"
	);
	cr_assert(eq(int, num_playlists, 4), "Expected 4 playlists");
	
	Playlist *playlist = playlists[3];
	cr_assert(not(zero(ptr, playlist->head)), "Expected playlist to have 1 element, got 0");
	cr_assert(eq(str, playlist->head->song.name, "Creep"), "Playlist's 1st song's name did not match expected");
	cr_assert(eq(str, playlist->head->song.artist, "Radiohead"), "Playlist's 1st song's artist did not match expected");
	cr_assert(eq(type(Genre), playlist->head->song.genre, ROCK), "Playlist's 1st song's genre did not match expected");

	cr_assert(zero(ptr, playlist->head->next), "Expected playlist to have 1 element, got more");
}