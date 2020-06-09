#include <utility>

/*!
 * @author: Joel Klimont
 * @filename: city.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::City::City(WINDOW* parent_window, std::string name, int pos_height, int pos_width,
                        int size) : GameObject(pos_height, pos_width, size, size*2), name(std::move(name)) {
    this->city_window = derwin(parent_window, height, width, pos_height, pos_width);
}

void game::gameobjects::City::draw(WINDOW* window) {
    box(this->city_window, 0 , 0);
    mvwaddstr(this->city_window, 0, 1, this->name.c_str());
    touchwin(this->city_window);
    wrefresh(this->city_window);

    for (auto & line : this->outgoing_connections) {
        line.draw(window);
    }
}

void game::gameobjects::City::tick() {
    for (auto & line : this->outgoing_connections) {
        line.tick();
    }
}

void game::gameobjects::City::add_incoming_track(const gameobjects::Track &track) {
    this->outgoing_connections.emplace_back(track);
}

void game::gameobjects::City::add_outgoing_track(const gameobjects::City &from, const gameobjects::Track &track) {
    this->incoming_connections.emplace_back(std::tuple<gameobjects::City, gameobjects::Track>(from, track));
}

bool game::gameobjects::City::is_point_within_city(int pos_height, int pos_width) {
    return pos_height >= this->pos_height && pos_height < this->pos_height + this->height
            && pos_width >= this->pos_width && pos_width < this->pos_width + this->width;
}
