#ifndef SHIPTYPE_H
#define SHIPTYPE_H

#include <string>
#include <vector>

#include "point.h"

using namespace std;

class ShipType
{
    private:
        string name;
        vector<Point>* points;

    public:
        ShipType(string, vector<Point>*);
        string getName();
        vector<Point>* getLocation();
        void setLocation(vector<Point>*);
};

#endif