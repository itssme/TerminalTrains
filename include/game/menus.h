#include <utility>

#include <utility>

/*!
 * @author: Joel Klimont
 * @filename: menus.h
 * @date: 31/05/20
*/

#ifndef TERMINALTRAINS_MENUS_H
#define TERMINALTRAINS_MENUS_H

#include "game.h"

#include <string>
#include <vector>

namespace game :: menu {

    struct Option {
        const std::string name;
        const std::string description;
        Option(std::string name, std::string description) : name(std::move(name)), description(std::move(description)) {}
    };

    class Menu {
    protected:
        std::vector<Option> options;
        std::vector<Menu> submenus;
        void draw(WINDOW* window) {};
    };

    class BuyTrain : Menu {
    protected:
        game::Game* game;
    public:
        BuyTrain(game::Game* game) {
            this->game = game;
        }
        void draw(WINDOW* window) {};
    };

    class EditTrain : Menu {
    protected:
        game::Game* game;
    public:
        EditTrain(game::Game* game) {
            this->game = game;
        }
        void draw(WINDOW* window) {};
    };

    class TrainMenu : Menu {
    protected:
        game::Game* game;
    public:
        TrainMenu(game::Game* game) {
            this->game = game;
            options.emplace_back(Option("Buy new Train", ""));
            options.emplace_back(Option("Edit Train", ""));
            options.emplace_back(Option("Exit", ""));

            submenus.emplace_back(BuyTrain(game));
            submenus.emplace_back(EditTrain(game));
        }
        void draw(WINDOW* window) {};
    };

    class GameMenu : Menu {
    protected:
        game::Game* game;
    public:
        GameMenu(game::Game* game) {
            this->game = game;
            options.emplace_back(Option("Trains", ""));
            options.emplace_back(Option("Exit", ""));

            submenus.emplace_back(TrainMenu(game));
        }
        void draw(WINDOW* window) {};
    };



}

#endif //TERMINALTRAINS_MENUS_H
