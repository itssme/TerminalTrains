/*! 
 * @author: Joel Klimont
 * @filename: curses_utilsV2.h
 * @date: 02/06/20
*/

#ifndef TERMINALTRAINS_GAME_MENUS_H
#define TERMINALTRAINS_GAME_MENUS_H

#include <mutex>
#include <string>
#include <vector>
#include <ncurses.h>

namespace game :: menu :: curses {
    /*!
    * An option which can be drawn in a menu.
    */
    struct Option {
    public:
        std::string title;
        std::string description;
        WINDOW *window;

        Option(WINDOW *parent_window, const std::string &title, const std::string &description, int length, int begin_y,
               int begin_x) {
            this->window = derwin(parent_window, 1, length - 1, begin_y, begin_x);
            this->title = title;
            this->description = description;
            waddstr(this->window, this->title.c_str());
        }

        /*!
         * Selecting the option will cause it being rendered
         * with reverse characters.
         */
        void select() {
            werase(this->window);
            wattroff(this->window, A_NORMAL);
            wattron(this->window, A_REVERSE);
            waddstr(this->window, this->title.c_str());
        }

        /*!
         * Deselecting the option will cause it being rendered
         * normal again.
         */
        void de_select() {
            werase(this->window);
            wattroff(this->window, A_REVERSE);
            wattron(this->window, A_NORMAL);
            waddstr(this->window, this->title.c_str());
        }

        /*!
         * Redraws the text
         */
        void redraw_subwin() {
            erase();
            waddstr(this->window, this->title.c_str());
        }

        /*!
         * Refresh subwindow of the option.
         */
        void refresh() {
            touchwin(this->window);
            wrefresh(this->window);
        }

        /*!
         * Returns the description such that it can be drawn in different WINDOW defined by the menu
         * @return The description of this option
         */
        std::string get_description() {
            return this->description;
        }

        /*!
         * Clear subwindow of the option.
         */
        void erase() {
            werase(this->window);
        }
    };

    /*!
     * An option which can be used to get text input from the user
     */
    struct OptionTextInput {
        std::string title;
        std::string text;
        int length;
        int input_size;
        WINDOW* window;
        WINDOW* text_window;
        OptionTextInput(WINDOW* parent_window, const std::string &title, int begin_y, int input_size) {
            this->length = static_cast<int>(title.size() + input_size + 2);
            this->window = derwin(parent_window, 1, this->length, begin_y, parent_window->_maxx/2 - this->length/2);
            this->title = title;
            waddstr(this->window, (this->title + " ").c_str());
            this->text_window = derwin(this->window, 1, input_size, 0, static_cast<int>(title.size() + 1));
            wattron(this->text_window, A_UNDERLINE);
            for (int i = 0; i < input_size; ++i) {
                waddch(this->text_window, ' ');
            }
            this->input_size = input_size;
        }
        /*!
         * Typed in chars will be interpreted here.
         * @param ch char the user typed in the terminal.
         */
        void pass_input(int ch) {
            if (ch != 91 && ch != 27) {
                if (ch == 127) {
                    if (this->text.length()) {
                        this->text.pop_back();
                    }
                } else {
                    this->text += static_cast<char>(ch);
                }
                this->redraw_subwin();
            }
        }
        /*!
         * Return the text the user typed in.
         * @return user input as string
         */
        std::string evaluate() {
            return this->text;
        }
        /*!
         * Redraw the option subwindow
         */
        void redraw_subwin() {
            werase(this->text_window);

            for (auto ch: this->text) {
                waddch(this->text_window, ch);
            }
            for (unsigned int i = 0; i < this->length - this->text.length(); ++i) {
                waddch(this->text_window, ' ');
            }
        }
        /*!
         * Selecting the option will cause it being rendered
         * with reverse characters.
         */
        void select() {
            werase(this->window);
            wattroff(this->window, A_NORMAL);
            wattron(this->window, A_REVERSE);
            waddstr(this->window, this->title.c_str());
            wattroff(this->window, A_REVERSE);
            waddstr(this->window, " ");
            redraw_subwin();
        }
        /*!
         * Deselecting the option will cause it being rendered
         * normal again.
         */
        void de_select() {
            werase(this->window);
            wattron(this->window, A_NORMAL);
            waddstr(this->window, this->title.c_str());
            waddstr(this->window, " ");
            redraw_subwin();
        }
        /*!
         * Refresh subwindow of the option.
         */
        void refresh() {
            touchwin(this->text_window);
            wrefresh(this->text_window);
            touchwin(this->window);
            wrefresh(this->window);
        }
        /*!
         * Clear subwindow of the option.
         */
        void erase() {
            werase(this->text_window);
            werase(this->window);
        }
    };

    /*!
     * Menu with multiple options.
     */
    class Menu {
    private:
        WINDOW *parent_window;
        std::string title;
    protected:
        unsigned long int at_option;
        int height;
        int width;
        int pos_y;
        int pos_x;
        bool has_decided;
        WINDOW* window;
        WINDOW* description_window;
        std::vector<Option> options{};
        std::vector<OptionTextInput> options_text_input{};
    public:
        Menu(WINDOW *parent_window, const std::vector<std::string> &option_names,
                const std::vector<std::string> &option_descriptions, const int &input_options,
                const std::string &title) {
            this->parent_window = parent_window;
            this->has_decided = false;
            this->title = title;
            keypad(stdscr, true);
            keypad(parent_window, true);

            this->window = derwin(parent_window, parent_window->_maxy, parent_window->_maxx, 0, 0);
            this->description_window = derwin(this->window, this->window->_maxy / 3, this->window->_maxx - 1, 1, 1);

            for (unsigned long int i = 0; i < option_names.size(); ++i) {
                if (i + 1 <= option_names.size() - input_options) {
                    this->options.emplace_back(Option(this->window, option_names.at(i), option_descriptions.at(i),
                                                      static_cast<int>(option_names.at(i).size()) + 1,
                                                      static_cast<int>(window->_maxy / 2 - option_names.size() / 2 + i),
                                                      static_cast<int>(window->_maxx / 2 -
                                                                       option_names.at(i).length() / 2)));
                } else {
                    this->options_text_input.emplace_back(OptionTextInput(this->window, option_names.at(i),
                                                                          static_cast<int>(window->_maxy / 2 -
                                                                                           option_names.size() / 2 + i),
                                                                          12));
                }
            }
            this->options.at(0).select();
            this->at_option = 0;
            redraw_all();
        }

        /*!
         * Update and refresh the terminal until
         * the user presses enter.
         * @param draw_mutex a mutex which has to be locked while drawing
         */
        void loop(std::mutex *draw_mutex = nullptr) {
            has_decided = false;
            timeout(-1);
            int ch;
            while ((ch = getch()) != 10) {
                if (ch == KEY_DOWN) {
                    this->down();
                } else if (ch == KEY_UP) {
                    this->up();
                } else {
                    this->pass_input(ch);
                }

                if (draw_mutex != nullptr) {
                    draw_mutex->lock();
                    this->draw_description();
                    this->refresh_all();
                    draw_mutex->unlock();
                } else {
                    this->draw_description();
                    this->refresh_all();
                }
            }
            timeout(0);
            has_decided = true;
        }

        /*!
         * Return size of the menu window
         * @return tuple that contains <height, width, pos_y, pos_x>
         */
        std::tuple<int, int, int, int> get_size() {
            return std::make_tuple(height, width, pos_y, pos_x);
        };

        /*!
         * Select the option above the current one
         * and deselect the one below.
         */
        void up() {
            if (at_option < options.size()) {
                this->options.at(at_option).de_select();
            } else {
                this->options_text_input.at(at_option - options.size()).de_select();
            }
            if (at_option == 0) {
                at_option = (options.size() + options_text_input.size()) - 1;
            } else {
                at_option--;
            }
            if (at_option < options.size()) {
                this->options.at(at_option).select();
            } else {
                this->options_text_input.at(at_option - options.size()).select();
            }
        }

        /*!
         * Deselect the option above the current one
         * and select the one below.
         */
        void down() {
            if (at_option < options.size()) {
                this->options.at(at_option).de_select();
            } else {
                this->options_text_input.at(at_option - options.size()).de_select();
            }
            if (at_option == (options.size() + options_text_input.size()) - 1) {
                at_option = 0;
            } else {
                at_option++;
            }
            if (at_option < options.size()) {
                this->options.at(at_option).select();
            } else {
                this->options_text_input.at(at_option - options.size()).select();
            }
        }

        /*!
         * Typed in chars will be interpreted here.
         * @param ch char the user typed in the terminal.
         */
        void pass_input(int ch) {
            if (at_option > this->options.size() - 1) {
                options_text_input.at(at_option - options.size()).pass_input(ch);
            }
        }

        /*!
         * Returns true if the user chose an option
         * @return true if evaluate or evaluate_choice can be called
         */
        bool can_evaluate() {
            return this->has_decided;
        }

        /*!
         * Return the selected option from the user
         * @return string of the selected option
         */
        std::string evaluate() {
            if (at_option < this->options.size()) {
                return this->options.at(at_option).title;
            } else {
                return this->options_text_input.at(at_option - options.size()).evaluate();
            }
        }

        /*!
         * Return the selected option from the user
         * @return int of the selected option
         */
        int evaluate_choice() {
            return static_cast<int>(at_option);
        }

        /*!
         * Refresh all windows and subwindows
         */
        void refresh_all() {
            for (auto op: options) {
                op.refresh();
            }
            for (auto op: options_text_input) {
                op.refresh();
            }
            touchwin(this->description_window);
            wrefresh(this->description_window);
            touchwin(this->window);
            wrefresh(this->window);
        }

        void draw_description() {
            werase(this->description_window);
            std::string description = options.at(at_option).get_description();

            if (description != "") {
                box(this->description_window, 0, 0);
                mvwaddstr(this->description_window, 0, 1, " Description ");

                int line_n{0};
                for (unsigned long int i = 0; i < description.length(); i += this->description_window->_maxx - 1) {
                    std::string line = description.substr(i, this->description_window->_maxx - 1);

                    // left trim
                    line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

                    mvwaddstr(this->description_window, line_n + 1, 1, line.c_str());
                    line_n++;
                }
            }
        }

        void redraw_all() {
            box(this->window, 0, 0);
            mvwaddstr(this->window, 0, 1, this->title.c_str());
            draw_description();
            for (auto op: options) {
                op.redraw_subwin();
            }
            for (auto op: options_text_input) {
                op.redraw_subwin();
            }
            refresh_all();
        }

        /*!
         * Refresh and erase all windows and subwindows
         */
        void erase() {
            for (auto op: options) {
                op.erase();
            }
            for (auto op: options_text_input) {
                op.erase();
            }
            werase(this->description_window);
            werase(this->window);
        }
    };
}

#endif //TERMINALTRAINS_GAME_MENUS_H
