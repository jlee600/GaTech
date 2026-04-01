#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../spotify.h"
#include "global.h"
#include "utils.h"

Test(play_next, null_queue) {
	play_next();
}

Test(play_next, success) {
	SongNode *a = heap_allocated_node(), *b = heap_allocated_node();
	b->next = NULL;
	b->song.artist = heap_allocated_string("this is");
	b->song.name = heap_allocated_string("the next song");
	b->song.genre = LOFI;

	a->next = b;
	a->song.artist = heap_allocated_string("jkl");
	a->song.name = heap_allocated_string("asdf");
	a->song.genre = COUNTRY;
	
	queue = a;
	play_next();

	cr_assert(eq(str, queue->song.name, "the next song"), "the next song should be at the front of the queue");
	cr_assert(eq(ptr, queue->next, NULL), "the next should be NULL");

	free_sn(b);
}
