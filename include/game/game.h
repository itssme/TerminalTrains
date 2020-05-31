/*! 
 * @author: Joel Klimont
 * @filename: game.h
 * @date: 29/05/20
*/

#ifndef TERMINALTRAINS_GAME_H
#define TERMINALTRAINS_GAME_H

#include "game_objects.h"

#define TICK 50

namespace game {

    class Game {
    private:
        int cursor_height;
        int cursor_width;
        int cursor_color;
        bool running{true};
        WINDOW* parent_window;
        WINDOW* menu_window;
        gameobjects::Map map;
        std::vector<gameobjects::City> cities;
        WINDOW* create_menu();
        void draw_cursor();
    public:
        Game(WINDOW *parent_window);
        void tick_all();
        void draw_all();
        void game_loop();
    };
}

#endif //TERMINALTRAINS_GAME_H
