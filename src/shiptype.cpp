#include "shiptype.h"

ShipType::ShipType
(
    string _name,
    vector<Point>* _points
)
:
    name(_name),
    points(_points)
{
    
}

string ShipType::getName()
{
    return name;
}

vector<Point>* ShipType::getLocation()
{
    return this->points;
}

void ShipType::setLocation(vector<Point>* ship)
{
    this->points = ship;
}