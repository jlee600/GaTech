#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(fight_ship, null_challenger)
{
	struct Ship temp_opp;
	cr_assert(eq(i32, fight_ship(NULL, &temp_opp), ERROR),
			  "Null ship passed for challenger, should return ERROR\n");
}

Test(fight_ship, null_opponent)
{
	struct Ship temp_chal;
	cr_assert(eq(i32, fight_ship(&temp_chal, NULL), ERROR),
			  "Null ship passed for opponent, should return ERROR\n");
}

Test(fight_ship, same_ships)
{
	struct Ship ship;
	memset(&ship, 0, sizeof(struct Ship));
	add_crew_member(&ship, "Vy", 10, CAPTAIN);
	add_crew_member(&ship, "Lofi", 9, SAILOR);
	add_crew_member(&ship, "Joever", 8, SAILOR);
	cr_assert(eq(i32, fight_ship(&ship, &ship), ERROR),
			  "Calling fight_ship on the same ship should return ERROR");
}

Test(fight_ship, challenger_win)
{
	struct Ship *stronger = &ships[0];
	struct Ship *weaker = &ships[1];
	ship_count = 2;

	strncpy(stronger->name, "Toyota Camry", MAX_NAME_LENGTH);
	strncpy(weaker->name, "Honda Civic", MAX_NAME_LENGTH);
	strncpy(ocean[0][0].ship_name, "Toyota Camry", MAX_NAME_LENGTH);
	strncpy(ocean[0][1].ship_name, "Honda Civic", MAX_NAME_LENGTH);

	add_member(stronger, "Vy", 14, CAPTAIN);
	add_member(stronger, "Henry", 15, QUARTERMASTER);
	add_member(weaker, "Marto", 9, CAPTAIN);
	add_member(weaker, "Matthew", 4, QUARTERMASTER);

	stronger->total_treasure = 10;
	weaker->total_treasure = 4;

	cr_assert(eq(i32, fight_ship(stronger, weaker), 1),
			  "Ship %s should have won this fight, with strength %d, against ship %s with strength %d",
			  stronger->name, 29, weaker->name, 13);

	cr_assert(eq(i32, ship_count, 1), "One ship should have been sank, only expecting 1 ship left");
	cr_assert(eq(i32, ships[0].crew[0].strength, 15), "%s of challenger should have leveled up their strength", "Vy");
	cr_assert(eq(i32, ships[0].crew[1].strength, 16), "%s of challenger should have leveled up their strength", "Henry");
	cr_assert(eq(i32, ships[0].total_treasure, 14), "Winning ship should acquire treasure from the sank ship");
}

Test(fight_ship, challenger_lose)
{
	struct Ship *stronger = &ships[0];
	struct Ship *weaker = &ships[1];
	ship_count = 2;

	strncpy(stronger->name, "Toyota Camry", MAX_NAME_LENGTH);
	strncpy(weaker->name, "Honda Civic", MAX_NAME_LENGTH);
	strncpy(ocean[0][0].ship_name, "Toyota Camry", MAX_NAME_LENGTH);
	strncpy(ocean[0][1].ship_name, "Honda Civic", MAX_NAME_LENGTH);

	add_member(stronger, "Yanderuri", 10, CAPTAIN);
	add_member(stronger, "Endorpersand", 10, QUARTERMASTER);
	add_member(weaker, "Martosis", 9, CAPTAIN);
	add_member(weaker, "Mfree", 9, QUARTERMASTER);

	stronger->total_treasure = 10;
	weaker->total_treasure = 8;

	cr_assert(eq(i32, fight_ship(weaker, stronger), 2),
			  "Ship %s should have won this fight, with strength %d, against ship %s with strength %d\n",
			  stronger->name, 20, weaker->name, 18);

	cr_assert(eq(i32, ship_count, 1), "One ship should have been sank, only expecting 1 ship left");
	cr_assert(eq(i32, ships[0].crew[0].strength, 11), "%s of challenger should have leveled up their strength\n", "Yanderuri");
	cr_assert(eq(i32, ships[0].crew[1].strength, 11), "%s of challenger should have leveled up their strength\n", "Endorpersand");
	cr_assert(eq(i32, ships[0].total_treasure, 18), "Winning ship should acquire treasure from the sank ship");
}
