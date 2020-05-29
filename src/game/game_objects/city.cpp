/*! 
 * @author: Joel Klimont
 * @filename: city.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

gameobjects::City::City(int pos_height, int pos_width,
                        int size) : GameObject(pos_height, pos_width, size*10, size*10) {}

void gameobjects::City::draw(WINDOW *window) {
    // TODO
}

void gameobjects::City::tick() {
    // TODO
}

void gameobjects::City::add_incoming_line(const gameobjects::Line& line) {
    this->outgoing_connections.emplace_back(line);
}

void gameobjects::City::add_outgoing_line(const gameobjects::City& from, const gameobjects::Line& line) {
    this->incoming_connections.emplace_back(std::tuple<gameobjects::City, gameobjects::Line>(from, line));
}