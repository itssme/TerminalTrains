/*! 
 * @author: Joel Klimont
 * @filename: game_objects.h
 * @date: 29/05/20
*/

#ifndef TERMINALTRAINS_GAME_OBJECTS_H
#define TERMINALTRAINS_GAME_OBJECTS_H

#include "colors.h"

#include <spdlog/spdlog.h>
#include <tuple>
#include <vector>
#include <ncurses.h>

namespace game::gameobjects {

    class GameObject {
    protected:
        // position
        int pos_height;
        int pos_width;

        // size
        int height;
        int width;

        // color
        unsigned int color{0};
    public:
        GameObject(int pos_height, int pos_width, int height, int width) : pos_height(pos_height), pos_width(pos_width),
                                                                         height(height), width(width) {}
        void tick() {};
        void draw(WINDOW* window) {};
    };

    // A cargo
    enum CargoType {
        Humans,
        Wood
    };

    // is transported on a Wagon
    class Wagon : public GameObject {
    private:
        int capacity;
        int load;
        CargoType cargo_type;
    public:
        Wagon(int capacity, CargoType cargo_type);
        void tick();
        void draw(WINDOW* window);
        void load_cargo();
        void load_cargo(int loading);
        int unload_cargo();
        int unload_cargo(int unload);
    };

    // which is part of a Train
    class Train : public GameObject {
    private:
        std::vector<Wagon> wagons;
        int position_in_line;
        std::vector<std::tuple<int, int>>* current_line;
    public:
        Train();
        void tick();
        void draw(WINDOW* window);
        void start_driving_on_line(std::vector<std::tuple<int, int>>* line);
        bool drive_line();
        void add_cargo();
        void add_cargo(const int& cargo);
        std::tuple<CargoType, int> unload_cargo();
    };

    // which drives on a Line
    class Line : public GameObject {
    private:
        Train* arrived_train = nullptr;
        std::vector<std::tuple<int, int>> line;
        std::vector<Train> trains;
    public:
        Line(int pos_height, int pos_width);
        void tick();
        void draw(WINDOW* window);
        void add_point(int pos_height, int pos_width);
        void remove_last_point();
        void add_train(Train train);
        int length();
        bool is_point_in_line(int pos_height, int pos_width);
        bool is_point_at_end_of_line(int pos_height, int pos_width);
        Train* arrived();
    };

    // from City to City
    class City : public GameObject {
    private:
        std::vector<Line> outgoing_connections;
        std::vector<std::tuple<City, Line>> incoming_connections;
        std::vector<Train> trains_at_city;
        WINDOW* city_window;
        std::string name;
    public:
        City(WINDOW* parent_window, std::string name, int pos_height, int pos_width, int size);
        void tick();
        void draw(WINDOW* window);
        void add_incoming_line(const Line& line);
        void add_outgoing_line(const City& from, const Line& line);
        bool is_point_within_city(int pos_height, int pos_width);
    };

    // on a huge map
    class Map : public GameObject {
    private:
        std::vector<City> cities;
        WINDOW* map_window;
    public:
        Map(WINDOW* parent_window, int height, int width);
        void tick();
        void draw(WINDOW* window);
        void add_city(const City& city);
        bool is_city_at(int pos_height, int pos_width);
        City* get_city_at(int pos_height, int pos_width);
        void add_line(City* from, City* to, Line* new_line);
    };
}

#endif //TERMINALTRAINS_GAME_OBJECTS_H
