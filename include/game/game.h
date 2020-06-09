/*! 
 * @author: Joel Klimont
 * @filename: game.h
 * @date: 29/05/20
*/

#ifndef TERMINALTRAINS_GAME_H
#define TERMINALTRAINS_GAME_H

#include "curses_utils.h"
#include "game_objects.h"
#include "menus.h"
#include "colors.h"

#define TICK 50

#define MENU_NONE 0
#define MENU_GAME 1
#define MENU_CITY 2
#define MENU_CONFIRM_TRACK 3

#define CURSOR_STATE_NONE 0
#define CURSOR_STATE_LINE 1

namespace game {

    struct LineBuilder {
        gameobjects::City* from;
        gameobjects::City* to;
        gameobjects::Track* new_track;
        LineBuilder();
    };

    class Game {
    private:
        int cursor_height;
        int cursor_width;
        int cursor_color;
        int cursor_state;
        int menu_type;
        bool running{true};
        std::mutex draw_mutex{};
        std::vector<gameobjects::Train> trains;
        WINDOW* parent_window;
        WINDOW* menu_window;
        Menu* menu = nullptr;
        menu::Menu* game_menu = nullptr;
        gameobjects::Map map;
        LineBuilder track_builder;
        void create_menu(const std::vector<std::string> &option_names, const int &input_options,
                         const std::string &title);
        std::thread* menu_thread;
        void draw_cursor();
    public:
        Game(WINDOW *parent_window);
        void tick_all();
        void draw_all();
        void game_loop();
        void add_train(gameobjects::Train train);
    };
}

#endif //TERMINALTRAINS_GAME_H
