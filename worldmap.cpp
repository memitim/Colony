#include "worldmap.h"

// Default map dimensions
int Worldmap::mapHeight = 128;
int Worldmap::mapWidth = 128;
int Worldmap::mapDepth = 20;

// Tile properties:
// 0: Sprite assigned to tile
// 1: Current animation frame of sprite
int Worldmap::tileProperties = 2;

std::vector< std::vector <std::vector< std::vector< signed int > > > > Worldmap::mapArray;

Worldmap::Worldmap()
{

}

Worldmap::~Worldmap()
{

}

// Map vector initialization
void Worldmap::initMapArray()
{
    /*
    // Quick and dirty map generation code chunk
    this->mapArray.resize(this->mapHeight);
    for(int mh=0;mh<mapHeight;++mh)
    {
    this->mapArray[mh].resize(this->mapWidth);
    for(int mw=0;mw<mapWidth;++mw)
    {
    this->mapArray[mh][mw].resize(this->mapDepth);
    for(int md=0;md<mapDepth;++md)
    {
    this->mapArray[mh][mw][md].resize(this->tileProperties);
    this->mapArray[mh][mw][md][1] = (rand()%2);
    }
    }
    }
    saveMap();
    */

    std::ifstream fileName("testmap.txt", std::ios::binary);
    if (fileName.is_open())
    {
        this->mapArray.resize(this->mapHeight);
        for (int mh = 0; mh<this->mapHeight; ++mh)
        {
            this->mapArray[mh].resize(this->mapWidth);
            for (int mw = 0; mw<mapWidth; ++mw)
            {
                this->mapArray[mh][mw].resize(this->mapDepth);
                for (int md = 0; md<mapDepth; ++md)
                {
                    this->mapArray[mh][mw][md].resize(this->tileProperties);
                    for (int tp = 0; tp<tileProperties; ++tp)
                    {
                        fileName.read((char *)(&mapArray[mh][mw][md][tp]), mapArray[mh][mw][md].size());
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "Failed to load map!" << std::endl;
    }
    fileName.close();
}

// Save map
void Worldmap::saveMap()
{
    std::ofstream fileName;
    fileName.open("testmap.txt", std::ios::binary);
    if (fileName.is_open())
    {
        for (int mh = 0; mh<mapHeight; ++mh)
        {
            for (int mw = 0; mw<mapWidth; ++mw)
            {
                for (int md = 0; md<mapDepth; ++md)
                {
                    for (int tp = 0; tp<tileProperties; ++tp)
                    {
                        fileName.write((char *)(&mapArray[mh][mw][md][tp]), mapArray[mh][mw][md].size());
                    }
                }
            }
        }
    }
    fileName.close();
}