#ifndef DATABASE_H
#define DATABASE_H

#include <bsoncxx/json.hpp>
#include <bsoncxx/document/value.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <map>

class Database
{
public:
    Database() = default;
    Database(const std::map<std::string, std::string> & databaseConfig);
    void registerTime(const bsoncxx::document::value & val);
    void runCommand(const bsoncxx::document::value & val);
private:
    mongocxx::instance inst{};
    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection timeEntries;
};

#endif // DATABASE_H
