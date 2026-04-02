#ifndef PIRATE
#define PIRATE

/* DO NOT MODIFY FILE */

#include <stdio.h>

#define UNUSED(x) ((void)x)
#define MAX_CREW_MEMBERS 20
#define MAX_NAME_LENGTH 20
#define MAX_SHIPS 20
#define OCEAN_WIDTH 20
#define OCEAN_HEIGHT 20

// Invalid values which are distinct from ERROR and NULL as to not give free points from the autograder
#define INCOMPLETE -2
#define INCOMPLETE_PTR(x) (void *) sizeof(x)
// Actual values to use
#define ERROR -1
#define SUCCESS 0

struct Location {
    int row;
    int col;
};

enum CrewRole { CAPTAIN, QUARTERMASTER, SAILOR };
struct CrewMember {
    char name[MAX_NAME_LENGTH];
    int strength;
    enum CrewRole role;
};

struct Ship {
    char name[MAX_NAME_LENGTH];

    // Crew attributes
    // Array of all crew members up to MAX_CREW_MEMBERS
    struct CrewMember crew[MAX_CREW_MEMBERS];
    // Number of crew members currently in the array
    int crew_size;

    // Treasure attribute
    int total_treasure;
};

struct OceanPatch {
    // If empty (e.g., starts with null terminator, there is no ship at this position)
    char ship_name[MAX_NAME_LENGTH];
    int treasure_amount;
};

enum Direction { LEFT, RIGHT, UP, DOWN };

// Crew management

int add_crew_member(struct Ship *ship, const char *name, int strength, enum CrewRole role);
struct CrewMember *find_crew_member(struct Ship *ship, const char *name);
int cast_overboard(struct Ship *ship, const char *name);
int mutiny(struct Ship *ship);

// Ship mechanics

int add_ship(const char *name, int row, int col);
struct Ship *get_ship(const char *name);
int sink_ship(struct Ship *ship);
int calculate_ship_strength(struct Ship *ship);
int fight_ship(struct Ship *challenger, struct Ship *opponent);

// Ocean mechanics

struct Location find_ship_location(struct Ship *ship);
int move_ship(struct Ship *ship, enum Direction direction);
int follow_direct_path(struct Ship *ship, int row, int col);
int bury_treasure(struct Ship *ship, int row, int col, int amount);
int find_hidden_treasure(struct Ship *ship);

extern struct Ship ships[MAX_SHIPS];
extern int ship_count;
extern struct OceanPatch ocean[OCEAN_HEIGHT][OCEAN_WIDTH];
#endif
