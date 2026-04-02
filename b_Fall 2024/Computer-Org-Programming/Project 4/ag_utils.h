#ifndef AG_UTILS_H
#define AG_UTILS_H

#include "pirate.h"
#include "string.h"
#include "math.h"

extern struct Ship ships[MAX_SHIPS];
extern int ship_count;
extern struct OceanPatch ocean[OCEAN_HEIGHT][OCEAN_WIDTH];

int count_roles(struct Ship *input, int *output);
int name_exists(struct Ship* input, char* name);
int count_ships_in_ocean(struct Ship* input);
int add_member(struct Ship *ship, const char *name, int strength, enum CrewRole role);
int prepare_ship_default(struct Ship* output);
int prepare_ship(struct Ship* output, const char* name, int crew_size, int total_treasure);
struct Ship* place_ship(struct Ship *ship, int row, int col);

char *cr_user_CrewRole_tostr(const enum CrewRole *role);
int cr_user_CrewRole_eq(const enum CrewRole *a, const enum CrewRole *b);
char *cr_user_CrewMember_tostr(const struct CrewMember *member);
int cr_user_CrewMember_eq(const struct CrewMember *a, const struct CrewMember *b);
char *cr_user_OceanPatch_tostr(const struct OceanPatch *patch);
int cr_user_OceanPatch_eq(const struct OceanPatch *a, const struct OceanPatch *b);
#endif
