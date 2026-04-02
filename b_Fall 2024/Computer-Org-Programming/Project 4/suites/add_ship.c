#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(add_ship, null_name) {
    cr_assert(eq(i32, add_ship(NULL, 0, 0), ERROR), "Null pointer passed for ship's name, should return ERROR");
    
    struct OceanPatch empty[OCEAN_WIDTH * OCEAN_HEIGHT] = { 0 };
    struct cr_mem cr_empty_ocean = { .data = empty, .size = sizeof(empty) };
    struct cr_mem cr_observed_ocean = { .data = ocean, .size = sizeof(ocean) };
    cr_assert(eq(mem, cr_observed_ocean, cr_empty_ocean), "Ocean should not have been modified");

    struct Ship empty_ships[MAX_SHIPS] = { 0 };
    struct cr_mem cr_empty_ships = { .data = empty_ships, .size = sizeof(empty_ships) };
    struct cr_mem cr_observed_ships = { .data = ships, .size = sizeof(ships) };
    cr_assert(eq(mem, cr_observed_ships, cr_empty_ships), "Ships array should not have been modified");

    cr_assert(eq(i32, ship_count, 0), "Ship count should not have been modified");
}

Test(add_ship, empty_name) {
    cr_assert(eq(i32, add_ship( "", 0, 0), ERROR), "Empty string passed for ship's name, should return ERROR");
    
    struct OceanPatch empty[OCEAN_WIDTH * OCEAN_HEIGHT] = { 0 };
    struct cr_mem cr_empty_ocean = { .data = empty, .size = sizeof(empty) };
    struct cr_mem cr_observed_ocean = { .data = ocean, .size = sizeof(ocean) };
    cr_assert(eq(mem, cr_observed_ocean, cr_empty_ocean), "Ocean should not have been modified");

    struct Ship empty_ships[MAX_SHIPS] = { 0 };
    struct cr_mem cr_empty_ships = { .data = empty_ships, .size = sizeof(empty_ships) };
    struct cr_mem cr_observed_ships = { .data = ships, .size = sizeof(ships) };
    cr_assert(eq(mem, cr_observed_ships, cr_empty_ships), "Ships array should not have been modified");

    cr_assert(eq(i32, ship_count, 0), "Ship count should not have been modified");
}

Test(add_ship, too_many_ships) {
    ship_count = MAX_SHIPS;
    cr_assert(eq(i32, add_ship("Ship", 0, 0), ERROR),
              "Number of ships exceeds MAX_SHIPS, should return ERROR.");

    struct OceanPatch empty[OCEAN_WIDTH * OCEAN_HEIGHT] = { 0 };
    struct cr_mem cr_empty_ocean = { .data = empty, .size = sizeof(empty) };
    struct cr_mem cr_observed_ocean = { .data = ocean, .size = sizeof(ocean) };
    cr_assert(eq(mem, cr_observed_ocean, cr_empty_ocean), "Ocean should not have been modified");

    struct Ship empty_ships[MAX_SHIPS] = { 0 };
    struct cr_mem cr_empty_ships = { .data = empty_ships, .size = sizeof(empty_ships) };
    struct cr_mem cr_observed_ships = { .data = ships, .size = sizeof(ships) };
    cr_assert(eq(mem, cr_observed_ships, cr_empty_ships), "Ships array should not have been modified");
    
    cr_assert(eq(i32, ship_count, MAX_SHIPS), "Ship count should not have been modified");
}

Test(add_ship, name_too_long) {
    char* too_long_name = "Ship name that doesn't fit into MAX_NAME_LENGTH characters.";
    cr_assert(eq(i32, add_ship(too_long_name, 0, 0), ERROR),
              "Ship name length exceeds MAX_NAME_LENGTH, should return ERROR.");
    
    struct OceanPatch empty[OCEAN_WIDTH * OCEAN_HEIGHT] = { 0 };
    struct cr_mem cr_empty_ocean = { .data = empty, .size = sizeof(empty) };
    struct cr_mem cr_observed_ocean = { .data = ocean, .size = sizeof(ocean) };
    cr_assert(eq(mem, cr_observed_ocean, cr_empty_ocean), "Ocean should not have been modified");

    struct Ship empty_ships[MAX_SHIPS] = { 0 };
    struct cr_mem cr_empty_ships = { .data = empty_ships, .size = sizeof(empty_ships) };
    struct cr_mem cr_observed_ships = { .data = ships, .size = sizeof(ships) };
    cr_assert(eq(mem, cr_observed_ships, cr_empty_ships), "Ships array should not have been modified");

    cr_assert(eq(i32, ship_count, 0), "Ship count should not have been modified");
}

Test(add_ship, out_of_bounds) {
    cr_assert(eq(i32, add_ship( "Theseus", -1, 0), ERROR), "Ship's row location is negative, should return ERROR");
    cr_assert(eq(i32, add_ship( "Theseus", 0, -1), ERROR), "Ship's column location is negative, should return ERROR");
    cr_assert(eq(i32, add_ship( "Theseus", OCEAN_HEIGHT, 0), ERROR), "Ship's row location >= OCEAN_HEIGHT, should return ERROR");
    cr_assert(eq(i32, add_ship( "Theseus", 0, OCEAN_WIDTH), ERROR), "Ship's column location >= OCEAN_WIDTH, should return ERROR");

    struct OceanPatch empty[OCEAN_WIDTH * OCEAN_HEIGHT] = { 0 };
    struct cr_mem cr_empty_ocean = { .data = empty, .size = sizeof(empty) };
    struct cr_mem cr_observed_ocean = { .data = ocean, .size = sizeof(ocean) };
    cr_assert(eq(mem, cr_observed_ocean, cr_empty_ocean), "Ocean should not have been modified");

    struct Ship empty_ships[MAX_SHIPS] = { 0 };
    struct cr_mem cr_empty_ships = { .data = empty_ships, .size = sizeof(empty_ships) };
    struct cr_mem cr_observed_ships = { .data = ships, .size = sizeof(ships) };
    cr_assert(eq(mem, cr_observed_ships, cr_empty_ships), "Ships array should not have been modified");

    cr_assert(eq(i32, ship_count, 0), "Ship count should not have been modified");
}

Test(add_ship, occupied_patch) {
    struct Ship existing_ship = { 0 };
    strncpy(existing_ship.name, ":o", MAX_NAME_LENGTH);
    strncpy(ocean[0][0].ship_name, existing_ship.name, MAX_NAME_LENGTH);
    ships[ship_count++] = existing_ship;
    cr_assert(eq(i32, add_ship("Ship", 0, 0), ERROR), "Ship created in occupied patch, should return ERROR.");

    struct OceanPatch orig[OCEAN_WIDTH * OCEAN_HEIGHT] = { 0 };
    strncpy(orig[0].ship_name, ":o", MAX_NAME_LENGTH);

    struct cr_mem cr_orig_ocean = { .data = orig, .size = sizeof(orig) };
    struct cr_mem cr_observed_ocean = { .data = ocean, .size = sizeof(ocean) };
    cr_assert(eq(mem, cr_observed_ocean, cr_orig_ocean), "Ocean should not have been modified");

    struct Ship expected_ships[MAX_SHIPS] = { 0 };
    expected_ships[0] = existing_ship;

    struct cr_mem cr_expected_ships = { .data = expected_ships, .size = sizeof(expected_ships) };
    struct cr_mem cr_observed_ships = { .data = ships, .size = sizeof(ships) };
    cr_assert(eq(mem, cr_observed_ships, cr_expected_ships), "Ships should not have been modified");

    cr_assert(eq(i32, ship_count, 1), "Ship count should not have been modified");
}

Test(add_ship, success) {
    cr_assert(eq(i32, add_ship("Ship", 1, 1), SUCCESS), "Basic ship add should return SUCCESS.");

    // Assert oceans equal:
    struct OceanPatch expected[OCEAN_WIDTH * OCEAN_HEIGHT] = { 0 };
    strncpy(expected[21].ship_name, "Ship", MAX_NAME_LENGTH);
    cr_assert(
        eq(type(struct OceanPatch)[OCEAN_WIDTH * OCEAN_HEIGHT], (struct OceanPatch *) ocean, expected),
        "Should have added ship to patch and left rest of ocean untouched"
    );

    // Assert ships equal:
    struct Ship expected_ship = { 0 };
    strncpy(expected_ship.name, "Ship", MAX_NAME_LENGTH);

    cr_assert(eq(i32, ship_count, 1), "Ship count should have been incremented");
    cr_assert(eq(str, ships[0].name, "Ship"), "Ship's name should match expected");
    cr_assert(eq(int, ships[0].crew_size, 0), "Ship's crew size should match expected");
    cr_assert(eq(int, ships[0].total_treasure, 0), "Ship's total treasure should match expected");
}
