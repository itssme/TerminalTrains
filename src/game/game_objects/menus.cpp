/*! 
 * @author: Joel Klimont
 * @filename: menus.cpp
 * @date: 10/06/20
*/

#include "game/menus.h"

int game::menu::BuyTrain::loop(WINDOW* parent_window, std::mutex* draw_mutex) {
    int result = game::menu::Menu::loop(parent_window, draw_mutex);

    switch (result) {
        case 0:
            (*this->game).add_train(gameobjects::Train());
            break;
        case 1:
            (*this->game).add_train(gameobjects::Train());
            break;
    }

    return result;
}

int game::menu::EditTrain::loop(WINDOW* parent_window, std::mutex* draw_mutex) {
    return Menu::loop(parent_window, draw_mutex);
}

int game::menu::TrainMenu::loop(WINDOW* parent_window, std::mutex* draw_mutex) {
    switch (Menu::loop(parent_window, draw_mutex)) {
        case 0:
            run_subwin(new BuyTrain(this->game), parent_window, draw_mutex);
            break;
        case 1:
            run_subwin(new EditTrain(this->game), parent_window, draw_mutex);
            break;
    }
    return -1;
}
