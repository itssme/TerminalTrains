/*! 
 * @author: Joel Klimont
 * @filename: line.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::Line::Line(int pos_height, int pos_width) : GameObject(pos_height, pos_width, 0, 0) {
    color = COLOR_PAIR(WHITE);
}

void game::gameobjects::Line::tick() {
    for (auto & train : trains) {
        if (! train.drive_line()) {
            arrived_train = &train;
        }
    }
}

void game::gameobjects::Line::draw(WINDOW* window) {
    wattron(window, color);
    for (auto & point : line) {
        mvwaddch(window, std::get<0>(point), std::get<1>(point), ' ');
    }
    wattroff(window, color);

    for (auto & train : trains) {
        train.draw(window);
    }
}

void game::gameobjects::Line::add_point(int pos_height, int pos_width) {
    this->line.emplace_back(std::make_tuple(pos_height, pos_width));
}

void game::gameobjects::Line::add_train(gameobjects::Train train) {
    train.start_driving_on_line(&this->line);
    this->trains.emplace_back(train);
}

game::gameobjects::Train *game::gameobjects::Line::arrived() {
    return arrived_train;
}

int game::gameobjects::Line::length() {
    return this->line.size();
}
