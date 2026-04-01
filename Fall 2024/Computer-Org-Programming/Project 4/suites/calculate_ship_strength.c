#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"

Test(calculate_ship_strength, null_ship) {
	cr_assert(eq(i32, calculate_ship_strength(0), ERROR), "Null ship passed should return ERROR");
}

Test(calculate_ship_strength, empty_crew) {
	struct Ship *ship = &ships[0];
	ship_count++;

	cr_assert(eq(i32, calculate_ship_strength(ship), 0), "Ship with no crew members should have 0 strength");
}

Test(calculate_ship_strength, full_crew) {
	struct Ship *ship = &ships[0];
	ship_count++;

	ship->crew_size = MAX_CREW_MEMBERS;

	for (int i = 0; i < MAX_CREW_MEMBERS; i++) {
		ship->crew[i].strength = i;
	}

	cr_assert(eq(i32, calculate_ship_strength(ship), MAX_CREW_MEMBERS * (MAX_CREW_MEMBERS - 1) / 2));
}

Test(calculate_ship_strength, partial_crew) {
	struct Ship *ship = &ships[0];
	ship_count++;

	ship->crew_size = 3;

	for (int i = 0; i < MAX_CREW_MEMBERS; i++) {
		ship->crew[i].strength = i;
	}

	cr_assert(eq(i32, calculate_ship_strength(ship), 3));
}
