#ifndef MISC_H
#define MISC_H

#include <string>
#include <stdexcept>
#include <random>
#include <iostream>

template<typename ... Args>
static std::string stringFormat(const std::string& format, Args ... args)
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ...) + 1;
    if( size_s <= 0 ){ throw std::runtime_error( "Formatting error occured." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ...);
    return std::string( buf.get(), buf.get() + size - 1 );
}

static int randomize(int low, int high)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(low, high);

    return (int)dist(mt);
}

static bool randomize()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    auto value = dist(mt);
    return (int)value % 2 == 0;
}

#endif