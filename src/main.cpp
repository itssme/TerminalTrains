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

#include "curses_utils.h"
#include "game/game.h"


int main() {
    try {
        auto logger = spdlog::basic_logger_mt("file_logger", "log.log");
        spdlog::set_default_logger(logger);
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
        return 1;
    }

    spdlog::info("started");
    spdlog::flush_on(spdlog::level::info);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(0);
    keypad(stdscr, true);
    start_color();
    refresh();

    bool in_menu{true};
    WINDOW* main_window = create_newwin(LINES, COLS, 0, 0);

    game::colors::init_colors();

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