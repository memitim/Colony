#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/System.hpp>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

typedef std::map<std::string, std::string> configMap;

class Config
{
public:
	Config();
	~Config();

	configMap settings;
	template<class T>
    T readSetting(const std::string& key)
   {
//		std::cout << settings[key] << std::endl;
		T value;
		std::istringstream keyStream(settings[key]);
		keyStream >> value;
		return value;
	}
	
private:
	void getSettings(const std::string& configFile = "config.ini");
	configMap Config::parseLine(std::string line);
};

#endif