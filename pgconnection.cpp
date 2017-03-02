#include "pgconnection.h"


PGConnection::PGConnection()
{
    m_connection.reset( PQsetdbLogin(m_dbhost.c_str(), std::to_string(m_dbport).c_str(), nullptr, nullptr, m_dbname.c_str(), m_dbuser.c_str(), m_dbpass.c_str()), &PQfinish );

    if (PQstatus( m_connection.get() ) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0 )
    {
       throw std::runtime_error( PQerrorMessage( m_connection.get() ) );
    }

}


std::shared_ptr<PGconn> PGConnection::connection() const
{
    return m_connection;
}

