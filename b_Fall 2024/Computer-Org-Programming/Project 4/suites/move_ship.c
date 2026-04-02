#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(move_ship, null_ship) {
    cr_assert(eq(i32, move_ship(NULL, LEFT), ERROR), "Null ship passed for ship should return ERROR");
}

Test(move_ship, out_of_bounds) {
    struct Ship ship;
    prepare_ship_default(&ship);
    struct Ship *placed_ship = place_ship(&ship, 0, 0);

    prepare_ship(&ship, "other", 0, 0);
    struct Ship *other_placed_ship = place_ship(&ship, OCEAN_HEIGHT - 1, OCEAN_WIDTH - 1);

    cr_assert(eq(i32, move_ship(placed_ship, LEFT), ERROR), "Moving ship off the ocean should return ERROR");
    cr_assert(eq(i32, move_ship(placed_ship, UP), ERROR), "Moving ship off the ocean should return ERROR");
    cr_assert(eq(i32, move_ship(other_placed_ship, RIGHT), ERROR), "Moving ship off the ocean should return ERROR");
    cr_assert(eq(i32, move_ship(other_placed_ship, DOWN), ERROR), "Moving ship off the ocean should return ERROR");
}

Test(move_ship, up) {
    struct Ship ship;
    prepare_ship_default(&ship);
    struct Ship *placed_ship = place_ship(&ship, 1, 0);

    cr_assert(eq(i32, move_ship(placed_ship, UP), SUCCESS), "Valid move should return SUCCESS");
    cr_assert(eq(str, ocean[1][0].ship_name, ""), "Old position should be empty after move");
    cr_assert(eq(str, ocean[0][0].ship_name, ship.name), "New position should be filled after move");
}

Test(move_ship, down) {
    struct Ship ship;
    prepare_ship_default(&ship);
    struct Ship *placed_ship = place_ship(&ship, 0, 0);
    
    cr_assert(eq(i32, move_ship(placed_ship, DOWN), SUCCESS), "Valid move should return SUCCESS");
    cr_assert(eq(str, ocean[0][0].ship_name, ""), "Old position should be empty after move");
    cr_assert(eq(str, ocean[1][0].ship_name, ship.name), "New position should be filled after move");
}

Test(move_ship, left) {
    struct Ship ship;
    prepare_ship_default(&ship);
    struct Ship *placed_ship = place_ship(&ship, 0, 1);
    
    cr_assert(eq(i32, move_ship(placed_ship, LEFT), SUCCESS), "Valid move should return SUCCESS");
    cr_assert(eq(str, ocean[0][1].ship_name, ""), "Old position should be empty after move");
    cr_assert(eq(str, ocean[0][0].ship_name, ship.name), "New position should be filled after move");
}

Test(move_ship, right) {
    struct Ship ship;
    prepare_ship_default(&ship);
    struct Ship *placed_ship = place_ship(&ship, 0, 0);
    
    cr_assert(eq(i32, move_ship(placed_ship, RIGHT), SUCCESS), "Valid move should return SUCCESS");
    cr_assert(eq(str, ocean[0][0].ship_name, ""), "Old position should be empty after move");
    cr_assert(eq(str, ocean[0][1].ship_name, ship.name), "New position should be filled after move");
}

Test(move_ship, lose_fight) {
    struct Ship ship;
    prepare_ship_default(&ship);
    add_member(&ship, "Blackbeard", 0xABCDEF, CAPTAIN);
    place_ship(&ship, 0, 0);

    struct Ship weak_ship;
    prepare_ship(&weak_ship, "Fishing Boat", 0, 0);
    add_member(&weak_ship, "Joe", 1, CAPTAIN);
    struct Ship *placed_ship = place_ship(&weak_ship, 0, 1);

    cr_assert(eq(i32, move_ship(placed_ship, LEFT), ERROR), "Losing move should return ERROR");
    cr_assert(eq(str, ocean[0][1].ship_name, ""), "Old ship position should be empty");
    cr_assert(eq(str, ocean[0][0].ship_name, ship.name), "Winning ship should remain");
}

Test(move_ship, win_fight) {
    struct Ship ship;
    prepare_ship_default(&ship);
    add_member(&ship, "Blackbeard", 0xABCDEF, CAPTAIN);
    struct Ship *placed_ship = place_ship(&ship, 0, 0);

    struct Ship weak_ship;
    prepare_ship(&weak_ship, "Fishing Boat", 0, 0);
    add_member(&weak_ship, "Joe", 1, CAPTAIN);
    place_ship(&weak_ship, 0, 1);

    cr_assert(eq(i32, move_ship(placed_ship, RIGHT), SUCCESS), "Winning move should return SUCCESS");
    cr_assert(eq(str, ocean[0][0].ship_name, ""), "Old ship position should be empty");
    cr_assert(eq(str, ocean[0][1].ship_name, ship.name), "New ship position should be filled with winner");
}