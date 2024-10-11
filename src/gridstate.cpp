#include "gridstate.h"

GridState::State GridState::getGridPositionStatus(int x, int y)
{
    return this->grid[x][y];
}


void GridState::setGridPositionStatus(int x, int y, State state)
{
    this->grid[x][y] = state;
}

int GridState::init()
{
    for(int i = 0; i <= this->size; i++)
    {
        vector<State> v1;
        for(int j = 0; i <= this->size; i++)
        {
            v1.push_back(State::FREE);
        }
        this->grid.push_back(v1);
    }
}
