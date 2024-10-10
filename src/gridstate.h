#ifndef GRID_STATE_H
#define GRID_STATE_H

#include <vector>

using namespace std;

class GridState {
    private:
        int size = 10;
        vector<vector<int>> grid;

    public:
        GridState()
        {
            grid = vector<vector<int>>();
        };

        void getGridPosition(int, int);
};

#endif