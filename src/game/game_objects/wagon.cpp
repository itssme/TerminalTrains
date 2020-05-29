/*! 
 * @author: Joel Klimont
 * @filename: wagon.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::Wagon::Wagon(int pos_height, int pos_width, int capacity, gameobjects::CargoType cargo_type)
        : GameObject(pos_height, pos_width, 1, 2), capacity(capacity), load(0),
          cargo_type(cargo_type) {}

void game::gameobjects::Wagon::tick() {

}

void game::gameobjects::Wagon::draw(WINDOW *window) {

}

void game::gameobjects::Wagon::load_cargo() {

}

void game::gameobjects::Wagon::load_cargo(int loading) {

}

int game::gameobjects::Wagon::unload_cargo() {
    return 0;
}

int game::gameobjects::Wagon::unload_cargo(int unload) {
    return 0;
}
