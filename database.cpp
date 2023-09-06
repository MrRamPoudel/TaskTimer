#include "database.h"
#include "helper.h"

Database::Database(std::map<std::string, std::string> & databaseConfig)
{
    const auto uri = mongocxx::uri{databaseConfig["MONGODB_URI"]};

    // Set the version of the Stable API on the client.
    mongocxx::options::client client_options;
    const auto api = mongocxx::options::server_api{ mongocxx::options::server_api::version::k_version_1 };
    client_options.server_api_opts(api);
}
