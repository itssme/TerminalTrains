#include <utility>

/*!
 * @author: Joel Klimont
 * @filename: menus.h
 * @date: 31/05/20
*/

#ifndef TERMINALTRAINS_MENUS_H
#define TERMINALTRAINS_MENUS_H

#include "game.h"
#include "curses_menus.h"

#include <string>
#include <vector>

namespace game {
    class Game;
}

namespace game :: menu {

    /*
    struct Option {
        const std::string name;
        const std::string description;
        Option(std::string name, std::string description) : name(std::move(name)), description(std::move(description)) {}
    };
     */

    class Menu {
    protected:
        int result{-1};
        std::string title;
        std::vector<std::string> option_names;
        std::vector<std::string> option_descriptions;
        std::vector<Menu> submenus;
        curses::Menu* menu = nullptr;
        void add_option(std::string name, std::string description) {
            this->option_names.emplace_back(name);
            this->option_descriptions.emplace_back(description);
        }
    public:
        virtual int loop(WINDOW* parent_window, std::mutex* draw_mutex) {
            menu = new curses::Menu(parent_window, option_names, option_descriptions, 0, title);
            menu->loop(draw_mutex);
            result = menu->evaluate_choice();
            menu = nullptr;
            return result;
        }
        bool can_evaluate() { return result != -1; }
        int evaluate() { return result; }
        void draw(WINDOW* window) {
            if (menu != nullptr) {
                menu->redraw_all();
            }
        };
    };

    class BuyTrain : public Menu {
    protected:
        game::Game* game;
    public:
        BuyTrain(game::Game* game) {
            this->game = game;
            add_option("Taurus", "Very good öbb train. Train very strong and very fast");
            add_option("Vectron", "Also very gud train and very strong but not so fast");
            add_option("Exit", "");
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override {
            return Menu::loop(parent_window, draw_mutex);
        }
    };

    class EditTrain : public Menu {
    protected:
        game::Game* game;
    public:
        EditTrain(game::Game* game) {
            this->game = game;
            add_option("Exit", "");
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override {
            return Menu::loop(parent_window, draw_mutex);
        }
    };

    class TrainMenu : public Menu {
    protected:
        game::Game* game;
    public:
        TrainMenu(game::Game* game) {
            this->game = game;
            add_option("Buy new Train", "");
            add_option("Edit Train", "");
            add_option("Exit", "");

            submenus.emplace_back(BuyTrain(game));
            submenus.emplace_back(EditTrain(game));
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override {
            switch (Menu::loop(parent_window, draw_mutex)) {
                case 0:
                    BuyTrain(this->game).loop(parent_window, draw_mutex);
                case 1:
                    EditTrain(this->game).loop(parent_window, draw_mutex);
            }
            return -1;
        }
    };

    class GameMenu : public Menu {
    protected:
        game::Game* game;
    public:
        GameMenu(game::Game* game) {
            this->game = game;
            add_option("Trains","");
            add_option("Exit", "");
            submenus.emplace_back(TrainMenu(game));
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override {
            switch (Menu::loop(parent_window, draw_mutex)) {
                case 0:
                    TrainMenu(this->game).loop(parent_window, draw_mutex);
            }
            return -1;
        }
    };
}

#endif //TERMINALTRAINS_MENUS_H
