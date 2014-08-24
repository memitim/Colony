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
	static Config* Instance();

	configMap settings;

	template<class T>
	T readSetting(const std::string& key)
	{
		T value;
		std::istringstream keyStream(settings[key]);
		keyStream >> value;
		return value;
	}

	template<class T>
	void writeSetting(const std::string& setting, const T& value)
	{
		std::ostringstream oss;
		oss << value;
		settings[setting] = oss.str();
	}

protected:
	Config();
	Config(const Config&);
	Config& operator= (const Config&);
	~Config();

private:
	static std::string configFile;

	void getSettings();
	configMap parseLine(std::string line);
	void saveSettings();
};

#endif