/*! 
 * @author: Joel Klimont
 * @filename: game.h
 * @date: 29/05/20
*/

#ifndef TERMINALTRAINS_GAME_H
#define TERMINALTRAINS_GAME_H

#include "curses_utils.h"
#include "game_objects.h"
#include "colors.h"

#define TICK 50

namespace game {

    class Game {
    private:
        int cursor_height;
        int cursor_width;
        int cursor_color;
        bool running{true};
        std::mutex draw_mutex{};
        WINDOW* parent_window;
        WINDOW* menu_window;
        Menu* menu;
        gameobjects::Map map;
        void create_menu(const std::vector<std::string> &option_names, const int &input_options,
                         const std::string &title);
        std::thread* menu_thread;
        void draw_cursor();
    public:
        Game(WINDOW *parent_window);
        void tick_all();
        void draw_all();
        void game_loop();
    };
}

#endif //TERMINALTRAINS_GAME_H
