#ifndef AG_UTILS_H
#define AG_UTILS_H

#include "../spotify.h"

extern int bytes_until_fail;
void free_sn(SongNode*);

char *cr_user_Genre_tostr(Genre *value);
int cr_user_Genre_eq(Genre *a, Genre *b);

typedef int Result;
char *cr_user_Result_tostr(Result *value);
int cr_user_Result_eq(Result *a, Result *b);

SongNode *heap_allocated_node(void);
Playlist *heap_allocated_playlist(void);
char *heap_allocated_string(char *str);

#endif
