#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(sink_ship, null_name) {
    cr_assert(eq(i32, sink_ship(NULL), ERROR), "Sinking NULL ship should return ERROR.");
}

Test(sink_ship, success) {
    struct Ship ship;
    prepare_ship_default(&ship);
    struct Ship *placed = place_ship(&ship, 1, 2);
    prepare_ship(&ship, "Other Boat", 0, 0);
    place_ship(&ship, 0, 0);

    struct OceanPatch expected_ocean[OCEAN_HEIGHT * OCEAN_WIDTH] = { 0 };
    memcpy(expected_ocean, ocean, sizeof(ocean));
    strcpy(expected_ocean[22].ship_name, "");

    // Assert placed ship was removed and that the remaining ship is "Other Boat":
    cr_assert(eq(i32, sink_ship(placed), SUCCESS), "Basic sink ship call should return SUCCESS.");
    cr_assert(eq(i32, ship_count, 1), "Ship count should have been decremented");
    cr_assert(eq(str, ships[0].name, ship.name), "Ship's name should match expected");
    cr_assert(eq(int, ships[0].crew_size, 0), "Ship's crew size should match expected");
    cr_assert(eq(int, ships[0].total_treasure, ship.total_treasure), "Ship's total treasure should match expected");

    // Assert oceans match:
    cr_assert(
        eq(type(struct OceanPatch)[OCEAN_HEIGHT * OCEAN_WIDTH], (struct OceanPatch *) ocean, expected_ocean),
        "Ocean should have removed sunk ship and otherwise remain the same"
    );
}