#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../pirate.h"
#include "../ag_utils.h"

Test(get_ship, null_name) {
    char *temp_name = 0;
    cr_assert(
        zero(ptr, get_ship(temp_name)),
        "Null pointer passed for name, should return NULL\n"
    );
}

Test(get_ship, not_found) {
    struct Ship temp_ship1;
    prepare_ship(&temp_ship1, "Sea Snake", 0, 0);
    ships[ship_count] = temp_ship1;
    ship_count++;

    struct Ship temp_ship2;
    prepare_ship(&temp_ship2, "Scallywag", 0, 0);
    ships[ship_count] = temp_ship2;
    ship_count++;

    cr_assert(
        zero(ptr, get_ship("Merchant")),
        "Passed in ship name does not exist, should return NULL\n"
    );
}

Test(get_ship, success) {
    struct Ship temp_ship1;
    prepare_ship(&temp_ship1, "Sea Snake", 0, 0);
    ships[ship_count] = temp_ship1;
    ship_count++;

    struct Ship temp_ship2;
    prepare_ship(&temp_ship2, "Scallywag", 0, 0);
    ships[ship_count] = temp_ship2;
    ship_count++;

    cr_assert(
        eq(ptr, get_ship("Scallywag"), &ships[ship_count - 1]),
        "Passed in Ship name exists, should return pointer to Ship\n"
    );
}