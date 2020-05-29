/*! 
 * @author: Joel Klimont
 * @filename: colors.h
 * @date: 29/05/20
*/

#ifndef TERMINALTRAINS_COLORS_H
#define TERMINALTRAINS_COLORS_H

#include <ncurses.h>

#define RED 1
#define GREEN 2
#define BLUE 3
#define MAGENTA 4
#define WHITE 5
#define BLACK 6
#define YELLOW 7

namespace game :: colors {
    void init_colors();
}

#endif //TERMINALTRAINS_COLORS_H
