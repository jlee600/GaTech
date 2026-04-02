#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(find_hidden_treasure, null_ship)
{
	cr_assert(
		eq(i32, find_hidden_treasure(NULL), ERROR),
		"Null pointer passed for ship, should return ERROR\n");
}

Test(find_hidden_treasure, no_treasure)
{
	struct Ship *temp_ship = &ships[0];
	strncpy(temp_ship->name, "Toyota C-HR", MAX_NAME_LENGTH);

	strncpy(ocean[0][0].ship_name, temp_ship->name, MAX_NAME_LENGTH);

	cr_assert(
		eq(i32, find_hidden_treasure(temp_ship), ERROR),
		"No treasure should be found");

	cr_assert(
		eq(i32, strcmp(temp_ship->name, ocean[0][0].ship_name), 0),
		"Ship should not move if no treasure was found");

	cr_assert(
		eq(i32, count_ships_in_ocean(temp_ship), 1),
		"Ship is either missing or duplicated, did you modify ocean[]?");
}

Test(find_hidden_treasure, fail_follow)
{
	struct Ship *player = &ships[0];
	struct Ship *obstacle = &ships[1];
	ship_count = 2;

	strncpy(player->name, "Toyota Camry", MAX_NAME_LENGTH);
	strncpy(obstacle->name, "Honda Civic", MAX_NAME_LENGTH);

	strncpy(ocean[0][0].ship_name, player->name, MAX_NAME_LENGTH);
	strncpy(ocean[0][1].ship_name, obstacle->name, MAX_NAME_LENGTH);

	player->total_treasure = 10;
	obstacle->total_treasure = 13;

	ocean[0][3].treasure_amount = 14;

	ship_count = 2;

	add_member(player, "Vy", 4, CAPTAIN);
	add_member(obstacle, "Marto", 8, CAPTAIN);
	ocean[0][4].treasure_amount = 10;

	cr_assert(eq(i32, find_hidden_treasure(player), ERROR),
			  "Ship should have been destroyed");
	cr_assert(eq(i32, obstacle->total_treasure, 23),
			  "Obstacle ship would have won and claim treasure");
	cr_assert(eq(i32, obstacle->crew[0].strength, 9),
			  "Crew member of obstacle ship should increment their strength after sinking enemy");
}

Test(find_hidden_treasure, success)
{
	struct Ship *player = &ships[0];
	strncpy(player->name, "Toyota Camry", MAX_NAME_LENGTH);
	strncpy(ocean[0][0].ship_name, player->name, MAX_NAME_LENGTH);
	player->total_treasure = 10;
	ocean[0][3].treasure_amount = 14;
	ship_count = 1;
	player->crew_size = 1;
	add_crew_member(player, "Vy", 4, CAPTAIN);
	ocean[0][4].treasure_amount = 10;

	cr_assert(eq(i32, find_hidden_treasure(player), SUCCESS),
			  "Ship should have found treasure at [0][3]");

	cr_assert(ne(i32, strcmp(ocean[0][0].ship_name, player->name), 0),
			  "Ship should have moved from its starting position\n");

	cr_assert(eq(i32, strcmp(ocean[0][3].ship_name, player->name), 0),
			  "Ship should have moved to new location to pick up treasure");

	cr_assert(eq(i32, player->total_treasure, 24),
			  "Ship should have updated its treasure.\n");

	ocean[2][3].treasure_amount = 18;

	cr_assert(eq(i32, find_hidden_treasure(player), SUCCESS),
			  "Ship should have found treasure at [2][3]");
	cr_assert(eq(i32, player->total_treasure, 42),
			  "Ship should have updated its treasure.");
	cr_assert(ne(i32, strcmp(ocean[0][3].ship_name, player->name), 0),
			  "Ship should have moved from its starting position");
	cr_assert(eq(i32, strcmp(ocean[2][3].ship_name, player->name), 0),
			  "Ship should have moved to new location to pick up treasure");
}