#ifndef GRID_STATE_H
#define GRID_STATE_H

#include <vector>

#include "point.h"
#include "shiptype.h"

using namespace std;

class GridState 
{
    private:
        int size;
        vector<vector<State>> grid;
        vector<ShipType> ships;

    public:
        GridState(int _size) : size(_size)
        {
            grid = vector<vector<State>>();
            init();
        };
        
        void init();
        State getGridPositionStatus(Point);
        void setGridPositionStatus(Point, State);
};

#endif