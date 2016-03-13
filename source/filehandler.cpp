#include <iostream> //Debug
#include <fstream>   //Read script files
#include <map> //Map for objects
#include <string> //For strings

#include "..\headers\filehandler.h"

namespace colony
{
    template <class genMedia>
    //Unload all assets
    void FileHandler::unloadAssets(std::map<std::string, genMedia>& genMap)
    {
        genMap.clear();
    }
    //Search and remove any spaces
    std::string FileHandler::strStripSpace(std::string stripper)
    {
        int spacePos;
        while(stripper.find(' ') != std::string::npos)
        {
            spacePos = stripper.find(' ');
            stripper.erase(spacePos,1);
        }
        while(stripper.find('\t') != std::string::npos)
        {
            spacePos = stripper.find('\t');
            stripper.erase(spacePos,1);
        }
        return stripper;
    }
    //Read line and output two strings
    bool FileHandler::strReadLine(std::string& strSource, std::string& strKey, std::string& strValue)
    {
        //Find and remove comment
        strSource = strSource.substr(0, strSource.find("\\"));

        //Is line empty?
        if(strSource == "")
            return false;

        //Quotation mark position array
        int qPos[4];

        //Find first quotation mark
        qPos[0] = strSource.find("\"");
        if(qPos[0] == -1)
            return false;

        //Find second, third and fourth quotation mark
        for(int i=1;i<=3;i++){
            qPos[i] = strSource.find("\"", qPos[i-1] + 1);
            if(qPos[i] == -1)
            {
                return false;
            }
        }

        //Set values
        strKey = strSource.substr(qPos[0] + 1,qPos[1] - qPos[0] -1);
        strValue = strSource.substr(qPos[2] + 1,qPos[3] - qPos[2] -1);

        return true;
	}
}