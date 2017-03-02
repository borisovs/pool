#include <thread>
#include <iostream>
#include "pgbackend.h"

void testConnection(std::shared_ptr<PGBackend> pgbackend)
{
    auto conn = pgbackend->connection();

    std::string demo = "SELECT max(id) FROM demo; " ;
    PQsendQuery( conn->connection().get(), demo.c_str() );

    while ( auto res_ = PQgetResult( conn->connection().get()) ) {
        if (PQresultStatus(res_) == PGRES_TUPLES_OK && PQntuples(res_)) {
            auto ID = PQgetvalue (res_ ,0, 0);
            std::cout<< ID<<std::endl;
        }

        if (PQresultStatus(res_) == PGRES_FATAL_ERROR){
            std::cout<< PQresultErrorMessage(res_)<<std::endl;
        }

        PQclear( res_ );
    }

    pgbackend->freeConnection(conn);

}


int main(int argc, char const *argv[])
{
	
	auto pgbackend = std::make_shared<PGBackend>();

    
    std::vector<std::shared_ptr<std::thread>> vec;

    for ( size_t i = 0; i< 50 ; ++i ){

        vec.push_back(std::make_shared<std::thread>(std::thread(testConnection, pgbackend)));
    }

    for(auto &i : vec) {
        i.get()->join();
    }

 
    return 0;
}
