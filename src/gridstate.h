#ifndef GRID_STATE_H
#define GRID_STATE_H

#include <vector>

using namespace std;

class GridState 
{
    public:
        enum State {
            FREE,
            SHIP,
            HIT,
            MISS
        };

    private:
        int size;
        vector<vector<State>> grid;
        

    public:
        GridState(int _size) : size(_size)
        {
            grid = vector<vector<State>>();
            init();
        };
        
        void init();
        State getGridPositionStatus(int, int);
        void setGridPositionStatus(int, int, State);
};

#endif