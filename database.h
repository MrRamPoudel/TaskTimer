#ifndef DATABASE_H
#define DATABASE_H

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <map>

class Database
{
public:
    Database(std::map<std::string, std::string> & databaseConfig);
    void registerTime(const bsoncxx::builder::basic::document & val);
private:
    mongocxx::instance inst{};
    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection timeEntries;
};

#endif // DATABASE_H
