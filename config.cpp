#include "Config.h"

#pragma warning( disable : 4996 )

std::string Config::configFile = "config.ini";

Config::Config()
{
	// Default settings in case file not present
	settings["title"] = "Colony";
	settings["width"] = "1920";
	settings["height"] = "1080";
	settings["fullscreen"] = "0";
	settings["framelimit"] = "1";
	settings["framerate"] = "60";
	settings["vsync"] = "0";
	settings["music_volume"] = "100";
	settings["sfx_volume"] = "100";
	settings["fov"] = "55";

	// Load settings
	getSettings();
}

Config::~Config()
{


}

Config* Config::Instance()

{
		static Config m_instance;
		return &m_instance;
}

// Load settings from file
void Config::getSettings()
{
	std::ifstream fileName(configFile);

	if (fileName.is_open())
	{
		std::string line;
		std::string key;
		std::string value;

		while (fileName.good())
		{
			getline(fileName, line);
			if (line.find("=", 0) == std::string::npos || line.compare(0, 1, ";") == 0)
			{
				// Blank, bad, or comment line. Skip it
			}
			else
			{
				configMap parsedLine = parseLine(line);
				if (parsedLine.size() > 0)
				for (std::map<std::string, std::string>::iterator iter = parsedLine.begin(); iter != parsedLine.end(); ++iter)
				{
					key = iter->first;
					value = iter->second;
					settings[key] = value;
				}
			}
		}
	}
	else
	{
		// TODO: Need error handler for failure to open file
	}

	fileName.close();
}

// Extract key and value from config file line
configMap Config::parseLine(std::string line)
{
	configMap parsedLine;
	size_t length1;
	size_t length2;
	char buffer1[50];
	char buffer2[50];
	std::string::size_type keyStart = line.find_first_not_of(" ", 0);
	std::string::size_type keyEnd = line.find_first_of("=");
	std::string::size_type valueStart = line.find_first_of("\"");
	std::string::size_type valueEnd = line.find_last_of("\"");
	length1 = line.copy(buffer1, keyEnd - keyStart, keyStart);
	length2 = line.copy(buffer2, valueEnd - valueStart - 1, valueStart + 1);
	buffer1[length1] = '\0';
	buffer2[length2] = '\0';
	parsedLine[std::string(buffer1)] = std::string(buffer2);
	return parsedLine;
}

void Config::saveSettings()
{
	std::ofstream fileName;
	fileName.open(configFile, std::ios::trunc);
	if (fileName.is_open())
	{
		std::string line;
		std::string key;
		std::string value;

		for (std::map<std::string, std::string>::iterator iter = settings.begin(); iter != settings.end(); ++iter)
			fileName << iter->first << "=\"" << iter->second << "\"" << std::endl;
	}
	fileName.close();
}