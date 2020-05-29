/*! 
 * @author: Joel Klimont
 * @filename: train.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

gameobjects::Train gameobjects::Line::arrived() {
    return trains.back(); // TODO: only for testing, change to real condition later
}

void gameobjects::Train::tick() {

}

void gameobjects::Train::draw(WINDOW *window) {

}

void gameobjects::Train::start_driving_on_line(int line_length) {

}

bool gameobjects::Train::drive_line() {
    return false;
}

gameobjects::Train::Train(int pos_height, int pos_width, int height, int width) : GameObject(pos_height, pos_width,
                                                                                             height, width) {}
