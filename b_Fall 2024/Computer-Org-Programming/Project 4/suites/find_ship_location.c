#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(find_ship_location, null_ship) {
    struct Ship *ship = 0;
    struct Location curr = find_ship_location(ship);
    cr_assert(
        eq(i32, curr.col, -1),
        "Null pointer passed for ship, should return a location struct with row = -1 and col = -1\n"
    );
    cr_assert(
        eq(i32, curr.row, -1),
        "Null pointer passed for ship, should return a location struct with row = -1 and col = -1\n"
    );
}

Test(find_ship_location, ship_not_found) {
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);
    struct Location curr = find_ship_location(&temp_ship);
    cr_assert(
        eq(i32, curr.col, -1),
        "Ship doesn't exist in ocean, should return a location struct with row = -1 and col = -1\n"
    );
    cr_assert(
        eq(i32, curr.row, -1),
        "Ship doesn't exist in ocean, should return a location struct with row = -1 and col = -1\n"
    );
}

Test(find_ship_location, success) {
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);
    struct OceanPatch *curr = &ocean[5][17];
    strncpy(curr->ship_name, temp_ship.name, MAX_NAME_LENGTH);
    struct Location currLoc = find_ship_location(&temp_ship);
    cr_assert(
        eq(i32, currLoc.col, 17),
        "Ship exists, should return a location struct with respective row and column\n"
    );
    cr_assert(
        eq(i32, currLoc.row, 5),
        "Ship exists, should return a location struct with respective row and column\n"
    );
}