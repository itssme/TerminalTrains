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

    class Menu {
    protected:
        int result{-1};
        std::string title = "";
        std::vector<std::string> option_names;
        std::vector<std::string> option_descriptions;
        std::vector<Menu> submenus;
        curses::Menu* menu = nullptr;
        Menu* submenu = nullptr;
        void add_option(std::string name, std::string description) {
            this->option_names.emplace_back(name);
            this->option_descriptions.emplace_back(description);
        }
    public:
        virtual int loop(WINDOW* parent_window, std::mutex* draw_mutex) {
            draw_mutex->lock();
            menu = new curses::Menu(parent_window, option_names, option_descriptions, 0, title);
            draw_mutex->unlock();

            menu->loop(draw_mutex);

            draw_mutex->lock();
            result = menu->evaluate_choice();
            menu = nullptr;
            draw_mutex->unlock();
            return result;
        }
        virtual bool can_evaluate() { return result != -1; }
        int evaluate() { return result; }
        void draw(WINDOW* window) {
            if (menu != nullptr) {
                menu->redraw_all();
            } else if (submenu != nullptr){
                submenu->draw(window);
            }
        };
        int run_subwin(Menu* subwin, WINDOW* parent_window, std::mutex* draw_mutex) {
            draw_mutex->lock();
            this->menu = nullptr;
            this->submenu = subwin;
            draw_mutex->unlock();
            return subwin->loop(parent_window, draw_mutex);
        }
    };

    class BuyTrain : public Menu {
    protected:
        game::Game* game;
    public:
        BuyTrain(game::Game* game) {
            this->game = game;
            this->title = "Buy Train";
            add_option("Taurus", "Very good obb train. Train very strong and very fast");
            add_option("Vectron", "Also very gud train and very strong but not so fast");
            add_option("Exit", "");
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override;
    };

    class EditTrain : public Menu {
    protected:
        game::Game* game;
    public:
        EditTrain(game::Game* game) {
            this->game = game;
            this->title = "Edit Train";
            add_option("Exit", "");
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override;
    };

    class TrainMenu : public Menu {
    protected:
        game::Game* game;
    public:
        TrainMenu(game::Game* game) {
            this->game = game;
            this->title = "Train Menu";
            add_option("Buy new Train", "Buy new Trains and Wagons");
            add_option("Edit Train", "Edit existing Trains");
            add_option("Exit", "");

            submenus.emplace_back(BuyTrain(game));
            submenus.emplace_back(EditTrain(game));
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override;
    };

    class GameMenu : public Menu {
    protected:
        bool done{false};
        game::Game* game;
    public:
        GameMenu(game::Game* game) {
            this->game = game;
            this->title = "Game Menu";
            add_option("Trains","Buy new Trains or edit Trains");
            add_option("Exit", "");
            submenus.emplace_back(TrainMenu(game));
        }
        int loop(WINDOW* parent_window, std::mutex* draw_mutex) override {
            switch (Menu::loop(parent_window, draw_mutex)) {
                case 0:
                    run_subwin(new TrainMenu(this->game), parent_window, draw_mutex);
                    break;
            }
            done = true;
            return -1;
        }
        bool can_evaluate() override {
            return done;
        }
    };
}

#endif //TERMINALTRAINS_MENUS_H
