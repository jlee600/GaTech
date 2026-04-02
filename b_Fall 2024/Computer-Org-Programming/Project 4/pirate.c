#include "pirate.h"
#include <string.h>

/*
 * Name: Jinseo Lee
 */

// Array of all ships up to MAX_SHIPS
struct Ship ships[MAX_SHIPS];
// Number of ships currently in the array
int ship_count;
// 2D array of all ocean patches with OCEAN_WIDTH width and OCEAN_HEIGHT height
struct OceanPatch ocean[OCEAN_HEIGHT][OCEAN_WIDTH];

//-- CREW MANAGEMENT --//

/**
 * Adds a new crew member to a given ship with the provided properties.
 * 
 * This function should not add a member and instead return ERROR if:
 * - the arguments were invalid (see PDF),
 * - the ship is currently full, or
 * - adding this crew member would result in duplicate captains or quartermasters
 * 
 * @param ship The ship to add the crew member to
 * @param name The name of the crew member
 * @param strength The amount of strength the crew member has
 * @param role The role the crew has in the ship
 * 
 * @returns SUCCESS if the crew member was successfully added, or
 *          ERROR if some error occurred (see above)
 */
int add_crew_member(struct Ship *ship, const char *name, int strength, enum CrewRole role) {
    if (!ship || !name || strength < 0) {return ERROR;}
    if (strlen(name) == 0 || strlen(name) >= MAX_NAME_LENGTH) {return ERROR;}
    // if first not captialized
    if (!(name[0] >= 'A' && name[0] <= 'Z')) {return ERROR;}
    // if the rest capitalized
    for (size_t i = 1; i < strlen(name); i++) {
        if (!(name[i] >= 'a' && name[i] <= 'z')) {return ERROR;}
    }
    if (ship->crew_size >= MAX_CREW_MEMBERS) {return ERROR;}
    // if captain already in the crew
    if (role == CAPTAIN) {
        for (int i = 0; i < ship->crew_size; i++) {
            if (ship->crew[i].role == CAPTAIN) {
                return ERROR;
            }
        }
    }
    // if quartermaster already in the crew
    if (role == QUARTERMASTER) {
        for (int i = 0; i < ship->crew_size; i++) {
            if (ship->crew[i].role == QUARTERMASTER) {
                return ERROR;
            }
        }
    }
    struct CrewMember new_member;
    strncpy(new_member.name, name, MAX_NAME_LENGTH);
    new_member.strength = strength;
    new_member.role = role; 

    ship->crew[ship->crew_size] = new_member;
    ship->crew_size++;
    return SUCCESS;
}

/**
 * Searches for a crew member in a given ship whose name matches the provided name.
 * 
 * This function should fail and return NULL if:
 * - the arguments were invalid (see PDF), or
 * - there does not exist a crew member with the provided name
 * 
 * @param ship The ship to search the crew member of
 * @param name The name of the crew member to search for
 * 
 * @returns a pointer to the CrewMember, or
 *          NULL if an error occurred (see above)
 */
struct CrewMember *find_crew_member(struct Ship *ship, const char *name) {
    if (!ship || !name) {
        return NULL;
    }
    for (int i = 0; i < ship->crew_size; i++) {
        int cmp = strcmp(ship->crew[i].name, name);
        if (cmp == 0) {
            return &ship->crew[i];
        }
    }
    return NULL;
}

/**
 * Removes the crew member with the given name from the ship.
 * 
 * In order to remove the crew member from the crew member array,
 * the element to remove should be replaced with the last element of the array 
 * and the array's size should be decremented. You should not shift the elements 
 * of the array forward. This process is specified in the PDF.
 * 
 * If the crew member being thrown overboard is of a high rank (i.e., not `SAILOR`),
 * then this function should also promote the first `SAILOR` in the crew array to that rank.
 * If no `SAILOR` exists on the ship, the crew member is not removed and ERROR is returned.
 * 
 * This function should not remove a crew member and instead return ERROR if:
 * - the arguments were invalid (see PDF),
 * - the crew member is not on the ship, or
 * - the crew member being thrown overboard is of high rank and there exists no `SAILOR` to replace their role
 * 
 * @param ship The ship to remove the crew member from
 * @param name The name of the crew member to remove
 * 
 * @returns SUCCESS if the crew member was successfully removed,
 *          ERROR if an error occurred (see above)
 */
int cast_overboard(struct Ship *ship, const char *name) {
    if (!ship || !name) {
        return ERROR;
    }
    if (find_crew_member(ship, name) == NULL) {
        return ERROR;
    }
    for (int i = 0; i < ship->crew_size; i++) {
        if (strcmp(ship->crew[i].name, name) == 0) {
            // if he is the sailor -> just remove
            if (ship->crew[i].role == SAILOR) {
                ship->crew[i] = ship->crew[ship->crew_size - 1];
                ship->crew_size--;
            } else { // if not, find the first sailor, promote him, and remove
                for (int j = 0; j < ship->crew_size; j++) {
                    if (ship->crew[j].role == SAILOR) {
                        ship->crew[j].role = ship->crew[i].role;
                        break;
                    }
                }
                ship->crew[i] = ship->crew[ship->crew_size - 1];
                ship->crew_size--;
            }
            break;
        }
    }
    return SUCCESS;
}

/**
 * Initiates a mutiny on the provided ship.
 * 
 * For a given ship, calculate the total strength of the crew members with names
 * starting `A-M` and of the crew members with names starting `N-Z`.
 * 
 * Then, cast overboard all crew members in the group with lesser strength. 
 * If there is a tie, do not cast any crew members overboard.
 * 
 * When casting the crew members overboard, loop through them in reverse order.
 * 
 * This function should return ERROR if:
 * - the arguments were invalid (see PDF), or
 * - an error occurred when using `cast_overboard`
 * 
 * @param ship The ship to perform the mutiny on
 * 
 * @returns SUCCESS if the ship was successfully mutinied,
 *          ERROR if an error occurred (see above)
 */
int mutiny(struct Ship *ship) {
    if (!ship) {
        return ERROR;
    }
    int strengthA = 0;
    int strengthB = 0;

    for (int i = 0; i < ship->crew_size; i++) {
        if (ship->crew[i].name[0] <= 'M') {
            strengthA += ship->crew[i].strength; 
        } else {
            strengthB += ship->crew[i].strength;
        }
    }
    if (strengthA < strengthB) {
        for (int i = ship->crew_size - 1; i >= 0; i--) {
            if (ship->crew[i].name[0] <= 'M') {
                cast_overboard(ship, ship->crew[i].name);
            }
        }
    } else {
        if (strengthA > strengthB) {
            for (int i = ship->crew_size - 1; i >= 0; i--) {
                if (ship->crew[i].name[0] > 'M') {
                    cast_overboard(ship, ship->crew[i].name);
                }
            }
        }
    }
    return SUCCESS;
}

//-- SHIP MANAGEMENT --//

/**
 * Adds a new ship to the global ship array and to the ocean grid.
 * 
 * This function should not add a new ship and instead return ERROR if:
 * - the arguments were invalid (see PDF),
 * - the global ship array is full, or
 * - the provided `(row, col)` points to an ocean patch that is out-of-bounds or occupied
 * 
 * @param name Name of the ship
 * @param row The row it appears in the ocean
 * @param col The column it appears in the ocean
 * 
 * @returns SUCCESS if the ship was successfully added,
 *          ERROR if the ship could not be successfully added (see above)
 */
int add_ship(const char *name, int row, int col) {
    if (!name || strlen(name) == 0) {return ERROR;}
    if (ship_count >= MAX_SHIPS) {return ERROR;}
    if (row < 0 || row >= OCEAN_HEIGHT || col < 0 || col >= OCEAN_WIDTH) {return ERROR;}
    if (ocean[row][col].ship_name[0] != '\0') {return ERROR;}
    if (strlen(name) >= MAX_NAME_LENGTH) {return ERROR;}

    strncpy(ocean[row][col].ship_name, name, MAX_NAME_LENGTH);
    ocean[row][col].treasure_amount = 0;

    strncpy(ships[ship_count].name, name, MAX_NAME_LENGTH);
    ships[ship_count].total_treasure = 0;

    ship_count++;
    return SUCCESS;
}

/**
 * Searches for a ship with the given name.
 * 
 * This function should fail and return NULL if:
 * - the arguments were invalid (see PDF), or
 * - there does not exist a ship with the provided name
 * 
 * @param name The name of the ship
 * 
 * @returns a pointer to the CrewMember, or
 *          NULL if an error occurred (see above)
 */
struct Ship *get_ship(const char *name) {
    if (!name || strlen(name) == 0) {return NULL;}
    for (int i = 0; i < ship_count; i++) {
        int cmp = strcmp(ships[i].name, name);
        if (cmp == 0) {
            return &ships[i];
        }
    }
    return NULL;
}

/**
 * Removes a ship from the global ship array and from the ocean grid.
 * 
 * In order to remove the ship from the ship array,
 * the element to remove should be replaced with the last element of the array 
 * and the array's size should be decremented. You should not shift the elements of the array forward.
 * 
 * @param ship Pointer to the ship to remove
 * 
 * @return SUCCESS if the ship was successfully removed,
 *         ERROR if the arguments were invalid (see PDF)
 */
int sink_ship(struct Ship *ship) {
    if (!ship) {return ERROR;}
    struct Location l = find_ship_location(ship);
    if (l.row == -1 && l.col == -1) {return ERROR;}
    for (int i = 0; i < ship_count; i++) {
        if (strcmp(ships[i].name, ship->name) == 0) {
            ships[i] = ships[ship_count - 1];
            ocean[l.row][l.col].ship_name[0] = '\0';
            ship_count--;
            break;
        }
    }
    return SUCCESS;
}

/**
 * Calculates the total strength of the ship. 
 * This is calculated by adding the strength of all crew members on the ship.
 * 
 * @param ship A pointer to the ship.
 * 
 * @returns the ship's strength, or
 *          ERROR if the arguments were invalid (see PDF)
 */
int calculate_ship_strength(struct Ship *ship) {
    if (!ship) {return ERROR;}
    int strength = 0;
    for (int i = 0; i < ship->crew_size; i++) {
        strength += ship->crew[i].strength;
    }
    return strength;
}

/**
 * Initiates a fight between two ships.
 * 
 * The algorithm for the fight is the following:
 * 1. Calculate the total strength of the two ships.
 * 2. Add all the treasure from the weaker ship to the stronger ship, based on strength.
 * 3. Increment the strength of each crew member in the stronger ship.
 * 4. Sink the losing ship.
 * 
 * If there is a tie in strength, there is no winner, and both ships are sunk.
 * 
 * This function should ERROR if:
 * - the arguments were invalid (see PDF), or
 * - the challenger is the same as the opponent
 * 
 * @param challenger The ship who starts the fight
 * @param opponent The ship who is the target for the fight
 * 
 * @returns 0 if there is a tie, 
 *          1 if the challenger wins, 
 *          2 if the opponent wins, or
 *          ERROR if the fight could not successfully occur (see above)
 */
int fight_ship(struct Ship *challenger, struct Ship *opponent) {
    if (!challenger || !opponent) {return ERROR;}
    if (challenger == opponent) {return ERROR;}

    int strengthC = calculate_ship_strength(challenger);
    int strengthO = calculate_ship_strength(opponent);

    if (strengthC == strengthO) {
        sink_ship(challenger);
        sink_ship(opponent);
        return 0;
    } else {
        if (strengthC > strengthO) {
            challenger->total_treasure += opponent->total_treasure;
            sink_ship(opponent);
            for (int i = 0; i < challenger->crew_size; i++) {
                challenger->crew[i].strength++;
            }
            return 1;
        } else {
            opponent->total_treasure += challenger->total_treasure;
            sink_ship(challenger);
            for (int i = 0; i < opponent->crew_size; i++) {
                opponent->crew[i].strength++;
            }
            return 2;
        }
    }
}

//-- OCEAN MECHANICS --//

/**
 * Finds the location of this ship. 
 * 
 * @param ship the ship to find the location of
 * 
 * @returns the location of the ship (the row and column), or
 *          `(row = -1, col = -1)` if arguments are invalid or ship does not exist in the ocean
 */
struct Location find_ship_location(struct Ship *ship) {
    struct Location l;
    l.row = -1;
    l.col = -1;
    if (!ship) {return l;}

    for (int i = 0; i < OCEAN_HEIGHT; i++) {
        for (int j = 0; j < OCEAN_WIDTH; j++) {
            int cmp = strcmp(ocean[i][j].ship_name, ship->name);
            if (cmp == 0) {
                l.row = i; 
                l.col = j; 
                break;
            }
        }
    }
    return l;
}

/**
 * Moves a ship one space in the specified direction.
 * 
 * If that position is occupied by another ship, fight the ship.
 * 
 * This function should not update the ship's position and raise ERROR if:
 * - the arguments were invalid (see PDF),
 * - `fight_ship` returns ERROR, or
 * - the current ship is destroyed during `fight_ship`
 * 
 * @param ship The ship to move
 * @param direction The direction to move
 * 
 * @return SUCCESS if ship was successfully moved to the new position, or
 *         ERROR if the ship could not be successfully moved (see above)
 */
int move_ship(struct Ship *ship, enum Direction direction) {
    struct Location l;
    if (!ship) {return ERROR;}
    // find the curr loc
    l = find_ship_location(ship);
    if (l.row == -1 && l.col == -1) {
        return ERROR;
    }
    ocean[l.row][l.col].ship_name[0] = '\0';
    // adjust the loc based on the dir
    if (direction == LEFT) {
        l.col--;
    } else if (direction == RIGHT) {
        l.col++;
    } else if (direction == UP) {
        l.row--;
    } else if (direction == DOWN) {
        l.row++;
    } else {
        return ERROR;
    }
    // invalid directions
    if (l.row < 0 || l.row >= OCEAN_HEIGHT || l.col < 0 || l.col >= OCEAN_WIDTH) {
        return ERROR;
    }
    // find the opponent
    struct Ship *op;
    for (int i = 0; i < ship_count; i++) {
        if (strcmp(ocean[l.row][l.col].ship_name, ships[i].name) == 0) {
            op = &ships[i];
            break;
        }
    }
    if (ocean[l.row][l.col].ship_name[0] != '\0') {
        int output = fight_ship(ship, op);
        if (output == 0 || output == 2) {
            return ERROR;
        } else if (output == 1) {
            strncpy(ocean[l.row][l.col].ship_name, ship->name, MAX_NAME_LENGTH);
        }
    } else {
        strncpy(ocean[l.row][l.col].ship_name, ship->name, MAX_NAME_LENGTH); 
    }
    return SUCCESS;
}

/**
 * PROVIDED FUNCTION
 * 
 * Moves a ship to the given row and column by iteratively moving one space at a time.
 * 
 * This function iteratively calls `move_ship` in one direction and then another in order to
 * implement this function.
 * 
 * This function returns ERROR if:
 * - the arguments were invalid (see PDF),
 * - `find_ship_location` returns an invalid location, or
 * - `move_ship` returns ERROR
 * 
 * @param ship The ship to move
 * @param row The row to end up at
 * @param col The column to end up at
 * 
 * @return SUCCESS if ship was successfully moved to the new position, or
 *         ERROR if the ship was not successfully moved to the new position (see above)
 */
int follow_direct_path(struct Ship *ship, int row, int col) {
    // Check if ship is NULL, or if the row and column are out of bounds
    if (!ship) return ERROR;
    if (!(0 <= row && row < OCEAN_HEIGHT)) return ERROR;
    if (!(0 <= col && col < OCEAN_WIDTH)) return ERROR;
    
    struct Location loc = find_ship_location(ship);

    // If ship is not found, return ERROR
    if (loc.row < 0 && loc.col < 0) {
        return ERROR;
    }

    int n_rows = row - loc.row;
    int n_cols = col - loc.col;

    // If we have a positive number of rows to traverse, move down until we get there.
    // If we have a negative number of rows to traverse, move up until we get there.
    if (n_rows > 0) {
        for (int i = 0; i < n_rows; i++) {
            if (move_ship(ship, DOWN) == ERROR) {
                return ERROR;
            }
        }
    } else if (n_rows < 0) {
        for (int i = 0; i < -n_rows; i++) {
            if (move_ship(ship, UP) == ERROR) {
                return ERROR;
            }
        }
    }

    // If we have a positive number of columns to traverse, move right until we get there.
    // If we have a negative number of columns to traverse, move left until we get there.
    if (n_cols > 0) {
        for (int i = 0; i < n_cols; i++) {
            if (move_ship(ship, RIGHT) == ERROR) {
                return ERROR;
            }
        }
    } else if (n_cols < 0) {
        for (int i = 0; i < -n_cols; i++) {
            if (move_ship(ship, LEFT) == ERROR) {
                return ERROR;
            }
        }
    }

    return SUCCESS;
}

/**
 * Moves the ship to the given row and column and buries treasure at the specified location.
 * 
 * In burying the treasure, this function removes the treasure amount from the ship and
 * adds it to the ocean patch at the specified location.
 * 
 * This function should return ERROR if:
 * - the arguments were invalid (see PDF),
 * - the ship does not have enough treasure (in which case the ship should not have moved), or
 * - any helper functions returned ERROR
 * 
 * @param ship The ship to dump treasure from
 * @param row The row to dump treasure at
 * @param col The column to dump treasure at
 * @param amount The total amount of treasure to dump
 * 
 * @return SUCCESS if ship was successfully moved to the position and treasure was successfully dumped, or
 *         ERROR if an error occurred (see above)
 */
int bury_treasure(struct Ship *ship, int row, int col, int amount) {
    if (!ship || row < 0 || row >= OCEAN_HEIGHT || col < 0 || col >= OCEAN_WIDTH) {
        return ERROR;
    }
    if (ship->total_treasure < amount || amount < 0) {
        return ERROR;
    }   
    follow_direct_path(ship, row, col);
    ship->total_treasure -= amount;
    ocean[row][col].treasure_amount += amount;

    return SUCCESS;
}

/**
 * Scans the ocean in search of treasure, moves to the position with the largest amount of treasure,
 * removes it from the ocean patch, and adds it to the ship.
 * 
 * This function should return ERROR if:
 * - the arguments were invalid (see PDF),
 * - there was no treasure found, or
 * - the ship did not successfully move to the new position
 * 
 * @return SUCCESS if ship was successfully moved to the position, or
 *         ERROR if an error occurred (see above)
 */
int find_hidden_treasure(struct Ship *ship) {
    if (!ship) {return ERROR;}
    int maxT = 0;
    int row;
    int col;

    for (int i = 0; i < OCEAN_HEIGHT; i++) {
        for (int j = 0; j < OCEAN_WIDTH; j++) {
            if (ocean[i][j].treasure_amount > maxT) {
                maxT = ocean[i][j].treasure_amount;
                row = i;
                col = j;
            }
        }
    }
    // if the ship didn't move
    if (maxT == 0) {
        return ERROR;
    }
    if (follow_direct_path(ship,row,col) == ERROR) {return ERROR;}
    ocean[row][col].treasure_amount = 0; 
    ship->total_treasure += maxT;
    
    return SUCCESS; 
}
