/*! 
 * @author: Joel Klimont
 * @filename: train.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

void game::gameobjects::Train::tick() {

}

void game::gameobjects::Train::draw(WINDOW *window) {

}

void game::gameobjects::Train::start_driving_on_line(int line_length) {
    this->position_in_line = line_length;
}

bool game::gameobjects::Train::drive_line() {
    return position_in_line-- != 0;
}

game::gameobjects::Train::Train(int pos_height, int pos_width, int height, int width) : GameObject(pos_height, pos_width,
                                                                                             height, width) {}
