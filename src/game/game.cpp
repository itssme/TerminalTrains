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
    this->trains.emplace_back(gameobjects::Train());
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
                    if (delta_height != 0 || delta_width != 0) {
                        if (line_builder.new_line->is_point_at_end_of_line(cursor_height + delta_height, cursor_width + delta_width)) {
                            line_builder.new_line->remove_last_point();
                            cursor_height += delta_height;
                            cursor_width += delta_width;
                        } else if (!line_builder.new_line->is_point_in_line(cursor_height + delta_height, cursor_width + delta_width)) {
                            line_builder.new_line->add_point(cursor_height, cursor_width);
                            cursor_height += delta_height;
                            cursor_width += delta_width;
                        }
                    }
                } else {
                    line_builder.new_line->add_point(cursor_height, cursor_width);

                    line_builder.to = city;
                    cursor_state = CURSOR_STATE_NONE;
                    menu_type = MENU_CONFIRM_LINE;

                    std::vector<std::string> main_menu_config;
                    main_menu_config.emplace_back("Name:");
                    main_menu_config.emplace_back("Create Line " + std::to_string(line_builder.new_line->length()));
                    main_menu_config.emplace_back("Exit");
                    this->create_menu(main_menu_config, 1, " Create Line ");
                }
            }

            if (enter_pressed && cursor_state == CURSOR_STATE_NONE && menu_type == MENU_NONE) {
                this->menu_type = MENU_GAME;
                this->game_menu = new menu::GameMenu(this);
                this->menu_window = derwin(parent_window, LINES-1, 25, 1, COLS - 26);
                this->menu_thread = new std::thread([&] {
                    game_menu->loop(menu_window, &draw_mutex);
                });
            }

        } else { // evaluate menu if possible

            if (menu != nullptr) {
                if (menu->can_evaluate()) {
                    menu_thread->join();
                    int menu_choice = menu->evaluate_choice();
                    std::vector<std::string> menu_text = menu->get_text_options();
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
                        if (menu_choice == 0 || menu_choice == 2) { // create actual line
                            line_builder.new_line->add_train(this->trains.at(0));
                            spdlog::info("line name " + menu_text.at(0)); // TODO: set line name in line
                            map.add_line(line_builder.from, line_builder.to, line_builder.new_line);
                        }
                    }
                }
            }

            if (game_menu != nullptr) {
                if (game_menu->can_evaluate()) {
                    menu_thread->join();
                    int menu_choice = game_menu->evaluate();
                    werase(menu_window);
                    menu_window = nullptr;
                    game_menu = nullptr;
                    menu_type = MENU_NONE;
                }
            }
        }

        auto end = std::chrono::steady_clock::now();

        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICK)-(end-start));
        } catch (const std::exception& e) {
            spdlog::info(e.what());
        }
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
        if (this->menu != nullptr) {
            this->menu->redraw_all();
        } else if (this->game_menu != nullptr) {
            this->game_menu->draw(parent_window);
        }
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
