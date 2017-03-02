
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include "pgbackend.h"

PGBackend::PGBackend()
{

    createPool();
  
}

void PGBackend::createPool()
{
    std::lock_guard<std::mutex> locker_( m_mutex );

    for ( auto i = 0; i< POOL; ++i ){
         m_pool.emplace ( std::make_shared<PGConnection>() );
    }
}

std::shared_ptr<PGConnection> PGBackend::connection()
{

    std::unique_lock<std::mutex> lock_( m_mutex );

    while ( m_pool.empty() ){
            m_condition.wait( lock_ );
    }

    auto conn_ = m_pool.front();
    m_pool.pop();

    return  conn_;
}


void PGBackend::freeConnection(std::shared_ptr<PGConnection> conn_)
{
    std::unique_lock<std::mutex> lock_( m_mutex );
    m_pool.push( conn_ );
    lock_.unlock();
    m_condition.notify_one();
}

