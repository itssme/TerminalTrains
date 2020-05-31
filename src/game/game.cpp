/*! 
 * @author: Joel Klimont
 * @filename: game.cpp
 * @date: 29/05/20
*/

#include "game/game.h"

game::Game::Game(WINDOW* parent_window) : parent_window{parent_window}, map(parent_window, 0, 0) {
    this->menu_window = nullptr;
    keypad(stdscr, true);
    keypad(parent_window, true);

    this->cursor_height = 2;
    this->cursor_width = 2;
    this->cursor_color = COLOR_PAIR(WHITE);

    this->cities.emplace_back(gameobjects::City(parent_window, 10, 10, 5));
    this->cities.emplace_back(gameobjects::City(parent_window, 16, 75, 5));
}

WINDOW *game::Game::create_menu() {
    return derwin(parent_window, LINES-1, 25, 1, COLS - 26);
}

void game::Game::draw_cursor() {
    wattron(this->parent_window, this->cursor_color);
    mvwaddch(this->parent_window, this->cursor_height, this->cursor_width, ' ');
    wattroff(this->parent_window, this->cursor_color);
}

void game::Game::game_loop() {
    while (this->running) {
        auto start = std::chrono::steady_clock::now();
        this->tick_all();
        this->draw_all();

        if (this->menu_window == nullptr) {
            wchar_t ch;
            bool height{true};
            bool width{true};

            while ((ch = getch()) != -1) {
                if (ch == KEY_UP && height) {
                    height = false;
                    if (cursor_height - 1)
                        cursor_height--;
                } else if (ch == KEY_DOWN && height) {
                    height = false;
                    if (cursor_height < LINES - 2)
                        cursor_height++;
                }

                if (ch == KEY_LEFT && width) {
                    width = false;
                    if (cursor_width - 1)
                        cursor_width--;
                } else if (ch == KEY_RIGHT && width) {
                    width = false;
                    if (cursor_width < COLS - 2)
                        cursor_width++;
                }
            }
        }

        auto end = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK)-(end-start));
    }
}

void game::Game::tick_all() {
    for (auto & city : cities) {
        city.tick();
    }
}

void game::Game::draw_all() {
    werase(parent_window);

    for (auto & city : cities) {
        city.draw(this->parent_window);
    }

    if (this->menu_window != nullptr) {
        touchwin(this->menu_window);
        wrefresh(this->menu_window);
    }

    draw_cursor();
    box(this->parent_window, 0 , 0);
    wrefresh(this->parent_window);
}
