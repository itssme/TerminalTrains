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
#include "types.h"
#include <thread>

#define TICK 50

#define MENU_NONE 0
#define MENU_GAME 1
#define MENU_CITY 2
#define MENU_CONFIRM_TRACK 3

#define CURSOR_STATE_NONE 0
#define CURSOR_STATE_LINE 1

namespace game::menu {
    class Menu;
    namespace curses {
        class Menu;
    }
}

namespace game {

    namespace containers {
        class Trains {
        private:
            std::vector<gameobjects::Train> trains;
        public:
            void add_train(gameobjects::Train train) { trains.emplace_back(train); };
            const std::vector<gameobjects::Train> &get_trains() { return trains; };
        };
    }

    struct LineBuilder {
        gameobjects::City* from;
        gameobjects::City* to;
        gameobjects::Track* new_track;
        LineBuilder();
    };

    class Game {
    private:
        uint cursor_height;
        uint cursor_width;
        uint cursor_color;
        uint cursor_state;
        uint menu_type;
        bool running{true};
        std::mutex draw_mutex{};
        std::vector<gameobjects::Train> trains;
        WINDOW* parent_window;
        WINDOW* menu_window;
        //Menu* menu = nullptr;
        menu::curses::Menu* custom_game_menu = nullptr;
        menu::Menu* game_menu = nullptr;
        gameobjects::Map map;
        LineBuilder track_builder;
        void
        create_menu(const std::vector<std::string> &option_names,
                    const int &input_options,
                    const std::string &title);
        std::thread* menu_thread;
        void draw_cursor();

    public:
        Game(WINDOW* parent_window);
        void tick_all();
        void draw_all();
        void game_loop();
        void add_train(gameobjects::Train train);
    };
}

#endif //TERMINALTRAINS_GAME_H
