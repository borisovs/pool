#ifndef PGCONNECTION_H
#define PGCONNECTION_H

#include <memory>
#include <mutex>
#include <libpq-fe.h>


class PGConnection
{
public:
    PGConnection();

    std::shared_ptr<PGconn> connection() const;

private:
    void establish_connection();

    std::string m_dbhost = "localhost";
    int         m_dbport = 5432;
    std::string m_dbname = "demo";
    std::string m_dbuser = "postgres";
    std::string m_dbpass = "postgres";

    std::shared_ptr<PGconn>  m_connection;

};


#endif //PGCONNECTION_H

