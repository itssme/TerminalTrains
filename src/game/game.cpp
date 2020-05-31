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

    this->map.add_city(gameobjects::City(parent_window, " Vienna ", 10, 10, 5));
    this->map.add_city(gameobjects::City(parent_window, " Linz ", 16, 75, 5));
}

void game::Game::create_menu(const std::vector<std::string> &option_names, const int &input_options,
                             const std::string &title) {
    this->menu_window = derwin(parent_window, LINES-1, 25, 1, COLS - 26);
    this->menu = new Menu(this->menu_window, option_names, input_options, title, 1, 1);
    this->menu_thread = new std::thread([&]{
        menu->loop(&draw_mutex);
    });
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
            int delta_height{0};
            int delta_width{0};

            while ((ch = getch()) != -1) {
                if (ch == KEY_UP && delta_height == 0) {
                    if (cursor_height - 1)
                        delta_height--;
                } else if (ch == KEY_DOWN && delta_height == 0) {
                    if (cursor_height < LINES - 2)
                        delta_height++;
                }

                if (ch == KEY_LEFT && delta_width == 0) {
                    if (cursor_width - 1)
                        delta_width--;
                } else if (ch == KEY_RIGHT && delta_width == 0) {
                    if (cursor_width < COLS - 2)
                        delta_width++;
                }
            }

            gameobjects::City* city = map.get_city_at(cursor_height + delta_height, cursor_width + delta_width);

            if (city == nullptr) {
                cursor_height += delta_height;
                cursor_width += delta_width;
            } else {
                std::vector<std::string> main_menu_config;
                main_menu_config.emplace_back("Create new Line");
                main_menu_config.emplace_back("City Settings");
                main_menu_config.emplace_back("Exit");

                this->create_menu(main_menu_config, 0, " City Menu ");
            }
        } else {
            if (menu->can_evaluate()) {
                menu_thread->join();
                spdlog::info("Use choose: " + std::to_string(menu->evaluate_choice()));
                werase(menu_window);
                menu_window = nullptr;
                menu = nullptr;
            }
        }

        auto end = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK)-(end-start));
    }
}

void game::Game::tick_all() {
    map.tick();
}

void game::Game::draw_all() {
    draw_mutex.lock();
    werase(parent_window);

    map.draw(parent_window);

    draw_cursor();
    box(this->parent_window, 0 , 0);

    if (this->menu_window != nullptr) {
        this->menu->redraw_all();
    }

    wrefresh(this->parent_window);
    draw_mutex.unlock();
}
