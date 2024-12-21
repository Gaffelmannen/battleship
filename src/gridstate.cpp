#include "gridstate.h"

State GridState::getGridPositionStatus(Point p)
{
    return this->grid[p.x][p.y];
}

void GridState::setGridPositionStatus(Point p, State state)
{
    this->grid[p.x][p.y] = state;
}

void GridState::init()
{
    for(int i = 0; i <= this->size; i++)
    {
        vector<State> v1;
        for(int j = 0; j <= this->size; j++)
        {
            v1.push_back(State::FREE);
        }
        this->grid.push_back(v1);
    }
}
