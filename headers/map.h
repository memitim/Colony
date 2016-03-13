#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "window.h"

class Map
{

public:
    Map();
    Map(int mapHeight, int mapWidth, int mapDepth);
    ~Map();

    void saveMap();
    void initMapArray();
    static std::vector< std::vector< std::vector< std::vector< signed int > > > > mapArray;

    // Dimensions of the map
    int mapHeight;
    int mapWidth;
    int mapDepth;
    // Number of tile properties
    static int tileProperties;

private:



    

};

#endif