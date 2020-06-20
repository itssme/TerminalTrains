#include <utility>

/*!
 * @author: Joel Klimont
 * @filename: city.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::City::City(WINDOW* parent_window, std::string name, uint pos_height, uint pos_width,
                              uint size) : GameObject(pos_height, pos_width, size, size*2), name(std::move(name)) {
    this->city_window = derwin(parent_window, height, width, pos_height, pos_width);
}

void game::gameobjects::City::draw(WINDOW* window) {
    box(this->city_window, 0 , 0);
    mvwaddstr(this->city_window, 0, 1, this->name.c_str());
    touchwin(this->city_window);
    wrefresh(this->city_window);

    for (auto & line : this->incoming_connections) {
        line.draw(window);
    }
}

void game::gameobjects::City::tick() {
    for (auto & line : this->incoming_connections) {
        line.tick();
    }
}

void game::gameobjects::City::add_incoming_track(gameobjects::Track track) {
    this->incoming_connections.emplace_back(track);
}

void game::gameobjects::City::add_outgoing_track(gameobjects::City* to, gameobjects::Track* track) {
    this->outgoing_connections.emplace_back(std::tuple<gameobjects::City*, gameobjects::Track*>(to, track));
}

bool game::gameobjects::City::is_point_within_city(uint pos_height, uint pos_width) {
    return pos_height >= this->pos_height && pos_height < this->pos_height + this->height
            && pos_width >= this->pos_width && pos_width < this->pos_width + this->width;
}
