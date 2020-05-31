/*! 
 * @author: Joel Klimont
 * @filename: train.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::Train::Train() : GameObject(0, 0, 1, 1) {
    color = COLOR_PAIR(RED);
}

void game::gameobjects::Train::tick() {}

void game::gameobjects::Train::draw(WINDOW* window) {
    wattron(window, color);
    mvwaddch(window, this->pos_height, this->pos_width, ' ');
    wattroff(window, color);
}

bool game::gameobjects::Train::drive_line() {
    if (position_in_line--) {
        std::tuple<int, int> position = current_line->at((current_line->size()-1) - position_in_line);
        this->pos_height = std::get<0>(position);
        this->pos_width = std::get<1>(position);
        return true;
    }
    return false;
}

void game::gameobjects::Train::start_driving_on_line(std::vector<std::tuple<int, int>>* line) {
    this->current_line = line;
    this->position_in_line = line->size();
}

void game::gameobjects::Train::add_cargo() {

}

void game::gameobjects::Train::add_cargo(const int &cargo) {

}

std::tuple<game::gameobjects::CargoType, int> game::gameobjects::Train::unload_cargo() {
    return std::tuple<game::gameobjects::CargoType, int>();
}
