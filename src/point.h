#ifndef POINT_H
#define POINT_H

class Point
{
    public:
        int x;
        int y;
        
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
};

#endif