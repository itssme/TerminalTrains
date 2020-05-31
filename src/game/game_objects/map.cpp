/*! 
 * @author: Joel Klimont
 * @filename: map.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::Map::Map(WINDOW* parent_window, int height, int width) : GameObject(0, 0, height, width) {
    this->window = parent_window;
}

void game::gameobjects::Map::tick() {
    for (auto & city : cities) {
        city.tick();
    }
}

void game::gameobjects::Map::draw(WINDOW *window) {
    for (auto & city : cities) {
        city.draw(window);
    }
}

void game::gameobjects::Map::add_city(const gameobjects::City& city) {
    cities.emplace_back(city);
}

bool game::gameobjects::Map::is_city_at(int pos_height, int pos_width) {
    for (auto & city: cities) {
        if (city.is_point_within_city(pos_height, pos_width)) {
            return true;
        }
    }

    return false;
}

game::gameobjects::City* game::gameobjects::Map::get_city_at(int pos_height, int pos_width) {
    for (auto & city: cities) {
        if (city.is_point_within_city(pos_height, pos_width)) {
            return &city;
        }
    }

    return nullptr;
}