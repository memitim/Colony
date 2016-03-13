#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <map>
#include <fstream>

namespace colony
{
    class FileHandler
    {
        public:
            FileHandler() {}
            ~FileHandler() {}

            template <class genMedia>
            //Unload all media
            void unloadAssets(std::map<std::string, genMedia>& genMap);
            //Search and remove any spaces
            static std::string strStripSpace(std::string stripper);
            //Read line and output two strings
            static bool strReadLine(std::string& strSource, std::string& strKey, std::string& strValue);

        protected:

    };
}


#endif