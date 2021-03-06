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
#include <utility>
#include <ncurses.h>

namespace game::gameobjects {

    class GameObject {
    protected:
        // position
        uint pos_height;
        uint pos_width;

        // size
        uint height;
        uint width;

        // color
        uint color{0};
    public:
        GameObject(uint pos_height, uint pos_width, uint height, uint width) : pos_height(pos_height), pos_width(pos_width),
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
        Wagon(uint capacity, CargoType cargo_type);
        void tick();
        void draw(WINDOW* window);
        void load_cargo();
        void load_cargo(uint amount);
        int unload_cargo();
        int unload_cargo(uint amount);
    };

    // which is part of a Train
    class Train : public GameObject {
    private:
        uint position_in_line;
        std::vector<Wagon> wagons;
        std::vector<std::tuple<uint, uint>>* current_line;
    public:
        Train();
        void tick();
        void draw(WINDOW* window);
        void start_driving_on_line(std::vector<std::tuple<uint, uint>>* line);
        bool drive_line();
        void add_cargo();
        void add_cargo(const uint& cargo);
        std::tuple<CargoType, uint> unload_cargo();
    };

    // which drives on a Track
    class Track : public GameObject {
    private:
        uint track_name_position{0};
        bool track_name_reversed{false};
        std::string track_name{""};
        Train* arrived_train = nullptr;
        std::vector<std::tuple<uint, uint>> track;
        std::vector<Train> trains;
    public:
        Track(uint pos_height, uint pos_width);
        void tick();
        void draw(WINDOW* window);
        void add_point(uint pos_height, uint pos_width);
        void remove_last_point();
        void add_train(Train train);
        uint length();
        bool is_point_on_track(uint pos_height, uint pos_width);
        bool is_point_at_end_of_track(uint pos_height, uint pos_width);
        void rename_track(std::string new_track_name);
        std::string get_track_name();
        Train* arrived();
    };

    // from City to City
    class City : public GameObject {
    private:
        std::vector<Track> incoming_connections;
        std::vector<std::tuple<City*, Track*>> outgoing_connections;
        WINDOW* city_window;
        std::string name;
    public:
        City(WINDOW* parent_window, std::string name, uint pos_height, uint pos_width, uint size);
        void tick();
        void draw(WINDOW* window);
        void add_incoming_track(Track track);
        void add_outgoing_track(City* to, Track* track);
        bool is_point_within_city(uint pos_height, uint pos_width);
    };

    // according to a Line
    class Line {
    private:
        std::vector<City*> line;
        std::vector<Track*> route;
        std::vector<Train*> trains;
    };

    // on a huge map
    class Map : public GameObject {
    private:
        std::vector<City> cities;
        WINDOW* map_window;
    public:
        Map(WINDOW* parent_window, uint height, uint width);
        void tick();
        void draw(WINDOW* window);
        void add_city(const City& city);
        bool is_city_at(uint pos_height, uint pos_width);
        City* get_city_at(uint pos_height, uint pos_width);
        void add_track(City* from, City* to, Track* new_track);
    };
}

#endif //TERMINALTRAINS_GAME_OBJECTS_H
