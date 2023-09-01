#ifndef HELPER_H
#define HELPER_H
#include <map>
#include <string>

class Helper
{
public:
    Helper();
    std::map<std::string, std::string> static readEnvFile(const std::string& fileName);
};

#endif // HELPER_H
