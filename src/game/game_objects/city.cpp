#include <utility>

/*!
 * @author: Joel Klimont
 * @filename: city.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::City::City(WINDOW* parent_window, std::string name, int pos_height, int pos_width,
                        int size) : GameObject(pos_height, pos_width, size, size*2), name(std::move(name)) {
    this->window = derwin(parent_window, height, width, pos_height, pos_width);
}

void game::gameobjects::City::draw(WINDOW* window) {
    box(this->window, 0 , 0);
    mvwaddstr(this->window, 0, 1, this->name.c_str());
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

bool game::gameobjects::City::is_point_within_city(int pos_height, int pos_width) {
    return pos_height >= this->pos_height && pos_height < this->pos_height + this->height
            && pos_width >= this->pos_width && pos_width < this->pos_width + this->width;
}
