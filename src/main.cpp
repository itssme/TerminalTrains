/*!
 * @author: Joel Klimont
 * @filename: main.cpp
 * @date: 26/05/20
*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#pragma GCC diagnostic pop

#include <iostream>
#include <chrono>

#include "game/game.h"
#include "game/curses_menus.h"

int main() {
    try {
        auto logger = spdlog::basic_logger_mt("file_logger", "log.log");
        spdlog::set_default_logger(logger);
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
        return 1;
    }

    spdlog::flush_on(spdlog::level::info);
    spdlog::info("started");

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(0);
    keypad(stdscr, true);
    start_color();
    refresh();

    bool in_menu{true};
    WINDOW* main_window = newwin(LINES, COLS, 0, 0);

    game::colors::init_colors();

    std::vector<std::string> main_menu_config;
    main_menu_config.emplace_back("Create new Line");
    main_menu_config.emplace_back("City Settings");
    main_menu_config.emplace_back("Exit");

    std::vector<std::string> description;
    description.emplace_back("Hello this is a very long text so this description is very long and does not fit into a line");
    description.emplace_back("");
    description.emplace_back("Exit the Menu");

    //game::menu::curses::Menu menu(main_window, main_menu_config, description, 0, " Menu ");

    //menu.loop();

    game::Game game(main_window);

    game.game_loop();

    /*
    while (in_menu) {
        int main_choice{0};

        { // MAIN MENU
            std::vector<std::string> main_menu_config;
            main_menu_config.emplace_back("Start game");
            main_menu_config.emplace_back("Settings");
            main_menu_config.emplace_back("Exit");
            Menu main_menu = Menu(menu_window, main_menu_config, 0, " TerminalTrains Menu ", 1, 1);
            main_menu.refresh_all();
            main_menu.loop();
            main_choice = main_menu.evaluate_choice();

            main_menu.erase();
            main_menu.refresh_all();
        }

        if (main_choice == 2) {
            in_menu = false;
        }

    }
     */

    endwin();
    return 0;
}