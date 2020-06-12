#include <utility>

#include <utility>

/*!
 * @author: Joel Klimont
 * @filename: track.cpp
 * @date: 29/05/20
*/

#include "game/game_objects.h"

game::gameobjects::Track::Track(int pos_height, int pos_width) : GameObject(pos_height, pos_width, 0, 0) {
    add_point(pos_height, pos_width);
    color = COLOR_PAIR(BLACK_WHITE);
}

void game::gameobjects::Track::tick() {
    for (auto & train : trains) {
        if (! train.drive_line()) {
            arrived_train = &train;
        }
    }
}

void game::gameobjects::Track::draw(WINDOW* window) {
    wattron(window, color);

    for (auto & point : track) {
        mvwaddch(window, std::get<0>(point), std::get<1>(point), ' ');
    }

    if (track_name_position && ! track_name.empty()) {
        const char* name = this->track_name.c_str();
        for (unsigned long int i = track_name_position; i < track_name_position + track_name.size(); ++i) {
            mvwaddch(window, std::get<0>(track.at(i % track.size())), std::get<1>(track.at(i % track.size())), *(name + sizeof(char) * (i - track_name_position)));
        }
    }

    wattroff(window, color);

    for (auto & train : trains) {
        train.draw(window);
    }
}

void game::gameobjects::Track::add_point(int pos_height, int pos_width) {
    this->track.emplace_back(std::make_tuple(pos_height, pos_width));
}

void game::gameobjects::Track::add_train(gameobjects::Train train) {
    train.start_driving_on_line(&this->track);
    this->trains.emplace_back(train);
}

game::gameobjects::Train* game::gameobjects::Track::arrived() {
    return arrived_train;
}

int game::gameobjects::Track::length() {
    return this->track.size();
}

bool game::gameobjects::Track::is_point_on_track(int pos_height, int pos_width) {
    if (track.empty()) {
        return false;
    }
    return std::find(track.begin(), track.end(), std::make_tuple(pos_height, pos_width)) != track.end();
}

bool game::gameobjects::Track::is_point_at_end_of_track(int pos_height, int pos_width) {
    if (track.empty()) {
        return false;
    }
    return track.back() == std::make_tuple(pos_height, pos_width);
}

void game::gameobjects::Track::remove_last_point() {
    if (! track.empty()) {
        track.pop_back();
    }
}

std::string game::gameobjects::Track::get_track_name() {
    std::string return_str{this->track_name};
    if (this->track_name_reversed) {
        std::reverse(return_str.begin(), return_str.end());
    }
    return return_str;
}

void game::gameobjects::Track::rename_track(std::string new_track_name) {
    this->track_name_position = 2;
    this->track_name = std::move(new_track_name);

    if (this->track_name.length() == 0) {
        return;
    }

    {
        int i{1};
        while (i < static_cast<int>(this->track.size()) - static_cast<int>(this->track_name.length()) - 1) {
            int desired_height{std::get<0>(this->track.at(i))};
            bool found_pos{true};

            for (unsigned long int j = 1; j < this->track_name.length(); ++j) {
                if (desired_height != std::get<0>(this->track.at(i + j))) {
                    found_pos = false;
                    i = i + j;
                    break;
                }
            }

            if (found_pos) {
                this->track_name_position = i + 1;
                break;
            }
        }
    }

    this->track_name_reversed = true;
    for (unsigned long int i = 1; i < this->track_name.length(); ++i) {
        if (std::get<1>(this->track.at((this->track_name_position + i - 1) % this->track.size())) <
                std::get<1>(this->track.at((this->track_name_position + i) % this->track.size()))) {
            this->track_name_reversed = false;
            break;
        }
    }

    if (this->track_name_reversed) {
        std::reverse(this->track_name.begin(), this->track_name.end());
    }
}
