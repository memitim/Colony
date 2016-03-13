#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "window.h"

class Worldmap
{

public:
    Worldmap();
    ~Worldmap();

    void saveMap();
    void initMapArray();
    static std::vector< std::vector< std::vector< std::vector< signed int > > > > mapArray;

    // Dimensions of the map
    static int mapHeight;
    static int mapWidth;
    static int mapDepth;
    // Number of tile properties
    static int tileProperties;

private:



    

};

#endif