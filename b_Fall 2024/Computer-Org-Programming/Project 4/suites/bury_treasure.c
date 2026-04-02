#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(bury_treasure, null_ship) {
    cr_assert(eq(i32, bury_treasure(NULL, 0, 0, 0), ERROR),
              "Bury treasure with NULL ship should return ERROR.");
}

Test(bury_treasure, insufficient) {
    add_ship("Ship", 0, 0);
    struct Ship* ship = get_ship("Ship");
    ship->total_treasure = 0;
    cr_assert(eq(i32, bury_treasure(ship, 0, 0, 5), ERROR),
              "Bury treasure with insufficient amount should return ERROR.");
    sink_ship(ship);
}

Test(bury_treasure, out_of_bounds) {
    add_ship("Ship", 0, 0);
    struct Ship* ship = get_ship("Ship");
    cr_assert(eq(i32, bury_treasure(ship, OCEAN_WIDTH, OCEAN_HEIGHT, 0), ERROR),
              "Bury treasure out of bounds should return ERROR.");
    sink_ship(ship);
}

Test(bury_treasure, success) {
    add_ship("Ship", 0, 0);
    struct Ship* ship = get_ship("Ship");
    ship->total_treasure = 10;
    cr_assert(eq(i32, bury_treasure(ship, 0, 0, 5), SUCCESS), "Basic bury treasure call should return SUCCESS.");
    sink_ship(ship);
}