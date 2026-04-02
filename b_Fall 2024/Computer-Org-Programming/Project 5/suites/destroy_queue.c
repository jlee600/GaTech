#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"
#include "utils.h"

Test(destroy_queue, null_queue) {
	destroy_queue();
}

// this has the same issue as destroy_playlist::success atm
Test(destroy_queue, success) {
	SongNode *a = heap_allocated_node(), *b = heap_allocated_node();
	a->next = b;
	char *c = malloc(5), *d = malloc(3);
	Song song = {.name = c, .artist = d, .genre = ROCK};
	a->song = song;

	b->next = NULL;
	char *e = malloc(3);
	char *f = malloc(5);
	Song other_song = {.name = e, .artist = f, .genre = ROCK};
	b->song = other_song;

	queue = a;
	destroy_queue();

	cr_assert(zero(queue), "Queue should have been successfully destroyed");
}
