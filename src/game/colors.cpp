/*!
 * @author: Joel Klimont
 * @filename: colors.cpp
 * @date: 29/05/20
*/

#include "game/colors.h"

void game::colors::init_colors() {
    init_pair(RED, COLOR_RED, COLOR_RED);
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
}
