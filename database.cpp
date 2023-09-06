#include "database.h"
#include <map>
#include <string>
#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/document/value.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

/*
 * Precondition: Pass map containing MONGODB_URI, DATABASE, COLLECTION
 *
 * Postconditon: Sets up the database instance with the provided config
 */

Database::Database(const std::map<std::string, std::string> & databaseConfig)
{
    const auto uri = mongocxx::uri{databaseConfig.at("MONGODB_URI")};
    // Set the version of the Stable API on the client.
    mongocxx::options::client client_options;
    const auto api = mongocxx::options::server_api{ mongocxx::options::server_api::version::k_version_1 };
    client_options.server_api_opts(api);
    client = mongocxx::client(uri, client_options);
    db = client[databaseConfig.at("DATABASE").c_str()];
    timeEntries = db[databaseConfig.at("COLLECTION").c_str()];
}

void Database::registerTime(const bsoncxx::document::value &val)
{
    try{
        timeEntries.insert_one(val.view());
        std::cout <<"Successfully inserted the value\n";
    }
    catch(std::exception & e){
        std::cerr<<e.what() <<'\n';
    }
}

void Database::runCommand(const bsoncxx::document::value &val)
{
    try{
        db.run_command(val.view());
        std::cout<<("Successfully ran the command");
    }
    catch(std::exception & e) {
        std::cerr<< e.what() <<'\n';
    }
}
