#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(find_crew_member, null_ship) {
	cr_assert(
		zero(ptr, find_crew_member(NULL, "Vy")),
		"Null pointer passed for ship, should return NULL\n"
	);
}

Test(find_crew_member, null_name) {
    struct Ship temp_ship;
    cr_assert(
        zero(ptr, find_crew_member(&temp_ship, NULL)),
        "Null pointer passed for name, should return NULL\n"
    );
}

Test(find_crew_member, not_found) {
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *name0 = "Eric";
    int mp0 = 10;
    int crewRole0 = QUARTERMASTER;
    add_member(&temp_ship, name0, mp0, crewRole0);

    char *name1 = "Kyle";
    int mp1 = 17;
    int crewRole1 = SAILOR;
    add_member(&temp_ship, name1, mp1, crewRole1);

    cr_assert(
        zero(ptr, find_crew_member(&temp_ship, "Timmy")),
        "The name passed does not exist, should return NULL\n"
    );
}

Test(find_crew_member, success) {
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *name0 = "Eric";
    int mp0 = 10;
    int crewRole0 = QUARTERMASTER;
    add_member(&temp_ship, name0, mp0, crewRole0);

    char *name1 = "Kyle";
    int mp1 = 17;
    int crewRole1 = SAILOR;
    add_member(&temp_ship, name1, mp1, crewRole1);

    struct CrewMember *kyle = &temp_ship.crew[1];
    cr_assert(
        eq(ptr, find_crew_member(&temp_ship, "Kyle"), kyle),
        "Crew member exists, should return pointer to Crew member\n"
    );
}