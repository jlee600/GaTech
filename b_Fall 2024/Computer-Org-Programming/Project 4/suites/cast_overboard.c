#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../ag_utils.h"
#include "../pirate.h"

Test(cast_overboard, null_ship) {
    cr_assert(eq(i32, cast_overboard(NULL, "5illy"), ERROR), "Null pointer passed in for ship should return ERROR");
}

Test(cast_overboard, null_name) {
    struct Ship ship;
    prepare_ship_default(&ship);
    cr_assert(eq(i32, cast_overboard(&ship, NULL), ERROR), "Null pointer passed in for name should return ERROR");
}

Test(cast_overboard, crewmember_not_found) {
    struct Ship ship;
    prepare_ship_default(&ship);
    cr_assert(eq(i32, cast_overboard(&ship, "i do not exist"), ERROR), "Casting non-existent member overboard should return ERROR");
}

Test(cast_overboard, success) {
    struct Ship ship;
    prepare_ship_default(&ship);
    add_member(&ship, "Apple", 1, CAPTAIN);
    add_member(&ship, "Banana", 1, QUARTERMASTER);
    add_member(&ship, "Cherry", 1, SAILOR);
    add_member(&ship, "Durian", 2, SAILOR);

    cr_assert(eq(i32, cast_overboard(&ship, "Cherry"), SUCCESS), "Valid crew member cast overboard should return SUCCESS");
    cr_assert(eq(i32, ship.crew_size, 3), "Crew size should have been decremented");
    
    for (int i = 0; i < ship.crew_size; i++) {
        cr_assert(ne(str, ship.crew[i].name, ""), "Did not keep crew contiguous");
    }
}

Test(cast_overboard, promote_captain) {
    struct Ship ship;
    prepare_ship_default(&ship);
    add_member(&ship, "Apple", 1, CAPTAIN);
    add_member(&ship, "Banana", 1, QUARTERMASTER);
    add_member(&ship, "Durian", 2, SAILOR);
    add_member(&ship, "Cherry", 1, SAILOR);

    cr_assert(eq(i32, cast_overboard(&ship, "Apple"), SUCCESS), "Valid crew member cast overboard should return SUCCESS");
    cr_assert(eq(i32, ship.crew_size, 3), "Crew size should have been decremented");
    
    for (int i = 0; i < ship.crew_size; i++) {
        if (!strcmp(ship.crew[i].name, "Durian")) {
            cr_assert(eq(i32, ship.crew[i].role, CAPTAIN), "Durian should have been promoted to CAPTAIN");
            return;
        }
    }

    cr_fail("Durian was not found in the crew.");
}

Test(cast_overboard, promote_quartermaster) {
    struct Ship ship;
    prepare_ship_default(&ship);
    add_member(&ship, "Apple", 1, CAPTAIN);
    add_member(&ship, "Banana", 1, QUARTERMASTER);
    add_member(&ship, "Durian", 2, SAILOR);
    add_member(&ship, "Cherry", 1, SAILOR);

    cr_assert(eq(i32, cast_overboard(&ship, "Banana"), SUCCESS), "Valid crew member cast overboard should return SUCCESS");
    cr_assert(eq(i32, ship.crew_size, 3), "Crew size should have been decremented");
    
    for (int i = 0; i < ship.crew_size; i++) {
        if (!strcmp(ship.crew[i].name, "Durian")) {
            cr_assert(eq(i32, ship.crew[i].role, QUARTERMASTER), "Durian should have been promoted to QUARTERMASTER");
            return;
        }
    }

    cr_fail("Durian was not found in the crew.");
}
