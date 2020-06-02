/*! 
 * @author: Joel Klimont
 * @filename: game.cpp
 * @date: 29/05/20
*/

#include "game/game.h"

game::Game::Game(WINDOW* parent_window) : parent_window{parent_window}, map(parent_window, 0, 0) {
    this->menu_window = nullptr;
    this->cursor_state = CURSOR_STATE_NONE;
    this->menu_type = MENU_NONE;

    keypad(stdscr, true);
    keypad(parent_window, true);

    this->cursor_height = 2;
    this->cursor_width = 2;
    this->cursor_color = COLOR_PAIR(YELLOW);

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
            bool enter_pressed{false};

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

                if (ch == 10) {
                    enter_pressed = true;
                }
            }

            gameobjects::City* city = map.get_city_at(cursor_height + delta_height, cursor_width + delta_width);

            if (cursor_state == CURSOR_STATE_NONE) {
                if (city == nullptr) {
                    cursor_height += delta_height;
                    cursor_width += delta_width;
                } else {
                    std::vector<std::string> main_menu_config;
                    main_menu_config.emplace_back("Create new Line");
                    main_menu_config.emplace_back("City Settings");
                    main_menu_config.emplace_back("Exit");

                    this->create_menu(main_menu_config, 0, " City Menu ");
                    menu_type = MENU_CITY;
                    line_builder.from = city;
                }
            } else if (cursor_state == CURSOR_STATE_LINE) {
                if (city == nullptr) {
                    cursor_height += delta_height;
                    cursor_width += delta_width;
                    line_builder.new_line->add_point(cursor_height, cursor_width);
                } else {
                    line_builder.to = city;
                    cursor_state = CURSOR_STATE_NONE;
                    menu_type = MENU_CONFIRM_LINE;

                    std::vector<std::string> main_menu_config;
                    main_menu_config.emplace_back("Create Line " + std::to_string(line_builder.new_line->length()));
                    main_menu_config.emplace_back("Exit");
                    this->create_menu(main_menu_config, 0, " Create Line ");
                }
            }

            if (enter_pressed && cursor_state == CURSOR_STATE_NONE && menu_type == MENU_NONE) {
                std::vector<std::string> main_menu_config;
                main_menu_config.emplace_back("Trains");
                main_menu_config.emplace_back("Exit");

                this->create_menu(main_menu_config, 0, " Game Menu ");
                this->menu_type = MENU_GAME;
            }

        } else { // evaluate menu if possible

            if (menu->can_evaluate()) {
                menu_thread->join();
                int menu_choice = menu->evaluate_choice();
                werase(menu_window);
                menu_window = nullptr;
                menu = nullptr;

                if (this->menu_type == MENU_CITY) {
                    menu_type = MENU_NONE;
                    if (menu_choice == 0) { // start creating new line
                        cursor_state = CURSOR_STATE_LINE;
                        line_builder.new_line = new gameobjects::Line(cursor_height, cursor_width);
                    } else if (menu_choice == 1) {
                        // TODO: city settings (don't forget setting menu_type)
                    }

                } else if (this->menu_type == MENU_CONFIRM_LINE) {
                    menu_type = MENU_NONE;
                    if (menu_choice == 0) { // create actual line
                        map.add_line(line_builder.from, line_builder.to, line_builder.new_line);
                    }

                } else if (this->menu_type == MENU_GAME) {
                    if (menu_choice == 0) { // create train menu
                        std::vector<std::string> main_menu_config;
                        main_menu_config.emplace_back("Buy new Train");
                        main_menu_config.emplace_back("Edit Train");
                        main_menu_config.emplace_back("Exit");

                        this->create_menu(main_menu_config, 0, " Train Menu ");
                        menu_type = MENU_TRAIN;
                    } else if (menu_choice == 1) {
                        // TODO
                    } else if (menu_choice == 2) {
                        menu_type = MENU_NONE;
                    }

                } else if (this->menu_type == MENU_TRAIN) {
                    if (menu_choice == 0) {
                        std::vector<std::string> main_menu_config;
                        main_menu_config.emplace_back("Taurus");
                        main_menu_config.emplace_back("Back");

                        this->create_menu(main_menu_config, 0, " Buy Train Menu ");
                        menu_type = MENU_BUY_TRAIN;
                    } else {
                        menu_type = MENU_NONE;
                    }
                } else if (this->menu_type == MENU_BUY_TRAIN) {

                }
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

    box(this->parent_window, 0 , 0);

    if (this->menu_window != nullptr) {
        this->menu->redraw_all();
    }

    if (this->cursor_state == CURSOR_STATE_LINE ||
        this->menu_type == MENU_CONFIRM_LINE) {
        this->line_builder.new_line->draw(parent_window);
    }

    draw_cursor();
    wrefresh(this->parent_window);
    draw_mutex.unlock();
}

game::LineBuilder::LineBuilder() {
    this->from = nullptr;
    this->to = nullptr;
    this->new_line = nullptr;

}
