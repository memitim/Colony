#include "window.h"
#include "game.h"

Window::Window()
{



}

Window::Window(sf::VideoMode videoMode, Config &config) : sf::RenderWindow(videoMode, config.readSetting<std::string>("title"))
{

}

Window::~Window()
{


}