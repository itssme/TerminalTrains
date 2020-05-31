/*! 
 * @author: Joel Klimont
 * @filename: city.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::City::City(WINDOW* parent_window, int pos_height, int pos_width,
                        int size) : GameObject(pos_height, pos_width, size*10, size*10) {
    this->window = derwin(parent_window, size, size, pos_height, pos_width);
}

void game::gameobjects::City::draw(WINDOW *window) {
    box(this->window, 0 , 0);
    touchwin(this->window);
    wrefresh(this->window);
}

void game::gameobjects::City::tick() {
    // TODO
}

void game::gameobjects::City::add_incoming_line(const gameobjects::Line& line) {
    this->outgoing_connections.emplace_back(line);
}

void game::gameobjects::City::add_outgoing_line(const gameobjects::City& from, const gameobjects::Line& line) {
    this->incoming_connections.emplace_back(std::tuple<gameobjects::City, gameobjects::Line>(from, line));
}
