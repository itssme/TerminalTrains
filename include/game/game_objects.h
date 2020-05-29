/*! 
 * @author: Joel Klimont
 * @filename: game_objects.h
 * @date: 29/05/20
*/

#ifndef TERMINALTRAINS_GAME_OBJECTS_H
#define TERMINALTRAINS_GAME_OBJECTS_H

#include <tuple>
#include <vector>
#include <ncurses.h>

namespace gameobjects {

    class GameObject {
    private:
        // position
        int pos_height;
        int pos_width;

        // size
        int height;
        int width;

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
        void tick();
        void draw(WINDOW *window);
        Wagon(int pos_height, int pos_width, int capacity, CargoType cargo_type);
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
    public:
        Train(int pos_height, int pos_width, int height, int width);
        void tick();
        void draw(WINDOW *window);
        void start_driving_on_line(int line_length);
        bool drive_line();
    };

    // which drives on a Line
    class Line : public GameObject {
    private:
        std::vector<std::tuple<int, int>> line;
        std::vector<Train> trains;
    public:
        Line(int pos_height, int pos_width);
        void tick();
        void draw(WINDOW *window);
        void add_point(int pos_height, int pos_width);
        void add_train(Train train);
        Train arrived();
    };

    // from City to City
    class City : public GameObject {
    private:

        std::vector<Line> outgoing_connections;
        std::vector<std::tuple<City, Line>> incoming_connections;
    public:
        City(int pos_height, int pos_width, int size);
        void add_incoming_line(const Line& line);
        void add_outgoing_line(const City& from, const Line& line);
        void tick();
        void draw(WINDOW *window);
    };

    // on a huge map
    //class Map : public GameObject {
    //    std::vector<City> cities;
    //};

}

#endif //TERMINALTRAINS_GAME_OBJECTS_H
