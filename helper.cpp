#include "helper.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>
Helper::Helper()
{

}

std::map<std::string, std::string> Helper::readEnvFile(const std::string & fileName) {
    std::map<std::string, std::string> envMap;
    std::ifstream file(fileName);
    if(!file.is_open()){
        std::cout<<("failed to open");
        exit(-1);
    }
    std::string line;
    while(std::getline(file, line)) {
        size_t pos = line.find('=');
        if(pos != std::string::npos) {
            envMap.insert(std::make_pair(line.substr(0,pos), line.substr(pos+1)));
        }
    }
    return envMap;
}
