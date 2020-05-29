/*! 
 * @author: Joel Klimont
 * @filename: map.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::Map::Map(int height, int width) : GameObject(0, 0, height, width) {}

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
