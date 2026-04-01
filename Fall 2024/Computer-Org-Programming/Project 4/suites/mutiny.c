#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(mutiny, null_ship)
{
	cr_assert(
		eq(i32, mutiny(NULL), ERROR),
		"Null pointer passed for ship, should return ERROR");
}

Test(mutiny, tie)
{
	struct Ship *temp_ship = &ships[0];
	ship_count++;
	add_member(temp_ship, "Matt", 10, CAPTAIN);
	add_member(temp_ship, "Vedh", 10, QUARTERMASTER);

	int expected_size = 2;

	cr_assert(
		eq(i32, mutiny(temp_ship), SUCCESS),
		"Nobody should have been thrown overboard, should return SUCCESS");

	cr_assert(
		eq(i32, temp_ship->crew_size, expected_size),
		"Mutiny was equal in power, crew size should be %d", expected_size);

	add_member(temp_ship, "Marto", 10, SAILOR);
	add_member(temp_ship, "Vy", 10, SAILOR);

	cr_assert(
		eq(i32, mutiny(temp_ship), SUCCESS),
		"Nobody should have been thrown overboard, should return SUCCESS");
	
	expected_size = 4;
	cr_assert(
		eq(i32, temp_ship->crew_size, expected_size),
		"Mutiny was equal in power, crew size should be %d", expected_size);
}

Test(mutiny, am_wins)
{
	struct Ship *temp_ship = &ships[0];
	ship_count++;
	add_member(temp_ship, "Matt", 12, CAPTAIN);
	add_member(temp_ship, "Vedh", 10, SAILOR);

	int expected_size = 2;
	cr_assert(eq(i32, temp_ship->crew_size, expected_size),
			  "Mutiny test cases depend on add_crew_member() being functional\n");

	int expected_roles[3] = {0, 0, 0};

	cr_assert(eq(i32, mutiny(temp_ship), SUCCESS),
			  "A-M crew have more power, at least somebody was thrown overboard");

	expected_size = 1;
	cr_assert(
		eq(i32, temp_ship->crew_size, expected_size),
		"Incorrect crew_size, did you cast_overboard() the losing crew?");

	expected_roles[CAPTAIN] = 1;
	expected_roles[QUARTERMASTER] = 0;
	expected_roles[SAILOR] = 0;

	int actual_roles[3] = {0, 0, 0};

	count_roles(temp_ship, actual_roles);

	for (int i = 0; i < 3; i++)
	{
		cr_assert(eq(i32, actual_roles[i], expected_roles[i]),
				  "Role %d doesn't match expected values, did you promote permissions in cast_overboard()?", i);
	}

	add_member(temp_ship, "Vedh", 10, SAILOR);
	add_member(temp_ship, "Marto", 20, QUARTERMASTER);
	add_member(temp_ship, "Vy", 10, SAILOR);

	cr_assert(
		eq(i32, mutiny(temp_ship), SUCCESS),
		 "A-M crew have more power, at least somebody was thrown overboard");

	expected_size = 2;

	cr_assert(
		eq(i32, temp_ship->crew_size, expected_size),
		"Incorrect crew_size, did you cast_overboard() the losing crew?");

	count_roles(temp_ship, actual_roles);
	expected_roles[CAPTAIN] = 1;
	expected_roles[QUARTERMASTER] = 1;
	expected_roles[SAILOR] = 0;

	for (int i = 0; i < 3; i++)
	{
		cr_assert(eq(i32, actual_roles[i], expected_roles[i]),
				  "Role %d (enum CrewRole) doesn't match expected values, did you promote permissions in cast_overboard()?", i);
	}
}

Test(mutiny, nz_wins)
{
	struct Ship *temp_ship = &ships[0];
	ship_count++;
	add_member(temp_ship, "Matt", 9, CAPTAIN);
	add_member(temp_ship, "Vedh", 12, SAILOR);
	add_member(temp_ship, "Vy", 12, SAILOR);

	int expected_size = 2;

	cr_assert(eq(i32, mutiny(temp_ship), SUCCESS),
			  "N-Z crew have more power, at least somebody was thrown overboard");
	cr_assert(
		eq(i32, temp_ship->crew_size, expected_size),
		"Incorrect crew_size, did you cast_overboard() the losing crew?");

	int expected_roles[3] = {1, 0, 1};
	int actual_roles[3] = {0, 0, 0};
	count_roles(temp_ship, actual_roles);

	for (int i = 0; i < 3; i++)
	{
		cr_assert(eq(i32, actual_roles[i], expected_roles[i]),
				  "Role %d (enum CrewRole) doesn't match expected values, did you promote permissions in cast_overboard()?");
	}

	add_member(temp_ship, "Matt", 8, SAILOR);
	add_member(temp_ship, "Varun", 14, SAILOR);
	add_member(temp_ship, "Marto", 4, QUARTERMASTER);

	cr_assert(eq(i32, mutiny(temp_ship), SUCCESS),
			  "N-Z crew have more power, at least somebody was thrown overboard");

	expected_size = 3;

	expected_roles[SAILOR] = 1;
	expected_roles[QUARTERMASTER] = 1;
	expected_roles[CAPTAIN] = 1;
	cr_assert(
		eq(i32, temp_ship->crew_size, expected_size),
		"Incorrect crew_size, did you cast_overboard() the losing crew?");

	count_roles(temp_ship, actual_roles);
	for (int i = 0; i < 3; i++)
	{
		cr_assert(eq(i32, actual_roles[i], expected_roles[i]),
				  "Role %d (enum CrewRole) doesn't match expected values, did you promote permissions in cast_overboard()?", i);
	};
}
