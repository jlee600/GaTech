#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(add_crew_member, null_ship)
{
    struct Ship *temp_ship = 0;

    char temp_name[3] = "Vy";
    int temp_mp = 5;
    int temp_crewRole = SAILOR;
    cr_assert(
        eq(i32, add_crew_member(temp_ship, temp_name, temp_mp, temp_crewRole),
           ERROR),
        "Null pointer passed for ship, should return ERROR");
}

Test(add_crew_member, null_name)
{
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *temp_name = 0;
    int temp_mp = 5;
    int temp_crewRole = SAILOR;
    cr_assert(
        eq(i32, add_crew_member(&temp_ship, temp_name, temp_mp, temp_crewRole), ERROR),
        "Null pointer passed for name, should return ERROR");
    
    cr_assert(eq(i32, temp_ship.crew_size, 0), "Crew size should be unchanged");
}

Test(add_crew_member, empty_name)
{
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *temp_name = "";
    int temp_mp = 5;
    int temp_crewRole = SAILOR;
    cr_assert(
        eq(i32, add_crew_member(&temp_ship, temp_name, temp_mp, temp_crewRole), ERROR),
        "Empty string passed for name, should return ERROR");
    
    cr_assert(eq(i32, temp_ship.crew_size, 0), "Crew size should be unchanged");
}

Test(add_crew_member, name_too_long)
{
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *temp_name = "Vyanderuri is definitely neovimming";
    int temp_mp = 5;
    int temp_crewRole = SAILOR;
    cr_assert(eq(i32, add_crew_member(&temp_ship, temp_name, temp_mp, temp_crewRole), ERROR), "Name is too long, should return ERROR");

    cr_assert(eq(i32, temp_ship.crew_size, 0), "Crew size should be unchanged");
}

Test(add_crew_member, name_incorrect_format)
{
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    // All lowercase
    cr_assert(eq(i32, add_crew_member(&temp_ship, "arstgmneio", 5, SAILOR), ERROR), "Name is of incorrect format, should return ERROR");
    // Numbers
    cr_assert(eq(i32, add_crew_member(&temp_ship, "Iii1i1ii1i", 5, SAILOR), ERROR), "Name is of incorrect format, should return ERROR");
    // Symbols
    cr_assert(eq(i32, add_crew_member(&temp_ship, "!@#$%^&*()", 5, SAILOR), ERROR), "Name is of incorrect format, should return ERROR");
    // All uppercase
    cr_assert(eq(i32, add_crew_member(&temp_ship, "QWFPBJLUYY", 5, SAILOR), ERROR), "Name is of incorrect format, should return ERROR");
    // One extraneous uppercase
    cr_assert(eq(i32, add_crew_member(&temp_ship, "AbcdefghiJ", 5, SAILOR), ERROR), "Name is of incorrect format, should return ERROR");
    // One extraneous symbol
    cr_assert(eq(i32, add_crew_member(&temp_ship, "@xcdvzkhxc", 5, SAILOR), ERROR), "Name is of incorrect format, should return ERROR");
    // One extraneous symbol (2)
    cr_assert(eq(i32, add_crew_member(&temp_ship, "X@cdvzkhxc", 5, SAILOR), ERROR), "Name is of incorrect format, should return ERROR");

    cr_assert(eq(i32, temp_ship.crew_size, 0), "Crew size should be unchanged");
}

Test(add_crew_member, full_ship)
{
    struct Ship ship;
    prepare_ship_default(&ship);
    ship.crew_size = MAX_CREW_MEMBERS;

    cr_assert(eq(i32, add_crew_member(&ship, "Name", 0, SAILOR), ERROR),
              "Number of crew members exceeds MAX_CREW_MEMBERS, should return ERROR.");

    cr_assert(eq(i32, ship.crew_size, MAX_CREW_MEMBERS), "Crew size should be unchanged");
}

Test(add_crew_member, negative_strength)
{
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *temp_name = "Vy";
    int temp_mp = -5;
    int temp_crewRole = SAILOR;
    cr_assert(
        eq(i32, add_crew_member(&temp_ship, temp_name, temp_mp, temp_crewRole), ERROR),
        "Negative strength passed, should return ERROR");

    cr_assert(eq(i32, temp_ship.crew_size, 0), "Crew size should be unchanged");
}

Test(add_crew_member, second_captain)
{
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *name0 = "Martosis";
    int mp0 = 10;
    int crewRole0 = CAPTAIN;
    cr_assert(eq(i32, add_crew_member(&temp_ship, name0, mp0, crewRole0), SUCCESS));

    char *name1 = "Vyanderuri";
    int mp1 = 17;
    int crewRole1 = CAPTAIN;
    cr_assert(eq(i32, add_crew_member(&temp_ship, name1, mp1, crewRole1), ERROR), "Adding a second captain, should return ERROR");

    cr_assert(eq(i32, temp_ship.crew_size, 1), "Crew size should be 1");
}

Test(add_crew_member, second_quartermaster)
{
    struct Ship temp_ship;
    prepare_ship_default(&temp_ship);

    char *name0 = "Endorpersand";
    int mp0 = 10;
    int crewRole0 = QUARTERMASTER;
    cr_assert(eq(i32, add_crew_member(&temp_ship, name0, mp0, crewRole0), SUCCESS));

    char *name1 = "Vyanderuri";
    int mp1 = 17;
    int crewRole1 = QUARTERMASTER;
    cr_assert(eq(i32, add_crew_member(&temp_ship, name1, mp1, crewRole1), ERROR), "Adding a second quartermaster, should return ERROR");

    cr_assert(eq(i32, temp_ship.crew_size, 1), "Crew size should be 1");
}

Test(add_crew_member, basic_success)
{
    struct Ship ship;
    prepare_ship_default(&ship);
    cr_assert(eq(i32, add_crew_member(&ship, "Owen", 315, SAILOR), SUCCESS), "Basic add Crew Member should return SUCCESS.");
    cr_assert(eq(i32, ship.crew_size, 1), "Crew size should have been incremented");
    cr_assert(eq(str, ship.crew[0].name, "Owen"), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].strength, 315), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].role, SAILOR));
}

Test(add_crew_member, captain_success)
{
    struct Ship ship;
    prepare_ship_default(&ship);
    cr_assert(eq(i32, add_crew_member(&ship, "Owen", 315, CAPTAIN), SUCCESS), "Basic add Crew Member should return SUCCESS.");
    cr_assert(eq(i32, ship.crew_size, 1), "Crew size should have been incremented");
    cr_assert(eq(str, ship.crew[0].name, "Owen"), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].strength, 315), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].role, CAPTAIN));
}

Test(add_crew_member, quartermaster_success)
{
    struct Ship ship;
    prepare_ship_default(&ship);
    cr_assert(eq(i32, add_crew_member(&ship, "Owen", 315, QUARTERMASTER), SUCCESS), "Basic add Crew Member should return SUCCESS.");
    cr_assert(eq(i32, ship.crew_size, 1), "Crew size should have been incremented");
    cr_assert(eq(str, ship.crew[0].name, "Owen"), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].strength, 315), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].role, QUARTERMASTER));
}

Test(add_crew_member, name_one_char)
{
    struct Ship ship;
    prepare_ship_default(&ship);
    cr_assert(eq(i32, add_crew_member(&ship, "A", 315, QUARTERMASTER), SUCCESS), "Add crew member with one-character name should return SUCCESS.");
    cr_assert(eq(i32, ship.crew_size, 1), "Crew size should have been incremented");
    cr_assert(eq(str, ship.crew[0].name, "A"), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].strength, 315), "Crew member should have been added");
    cr_assert(eq(i32, ship.crew[0].role, QUARTERMASTER));
}