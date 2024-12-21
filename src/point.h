#ifndef POINT_H
#define POINT_H

#include "state.h"

class Point
{
    public:
        int x;
        int y;
        State state;
        
        Point() 
        {}

        Point(
            int _x, 
            int _y
        )
        :
            x(_x),
            y(_y)
        {}
        
        Point operator+(Point const &obj)
        {
            Point res;
            res.x = x + obj.x;
            res.y = y + obj.y;
            return res;
        }

        bool operator==(Point const &obj)
        {
            return x == obj.x && y == obj.y;
        }

        bool operator!=(Point const &obj)
        {
            return x != obj.x || y != obj.y;
        }
};

#endif