#ifndef HASH_REGULAR_GRID_H
#define HASH_REGULAR_GRID_h

#include "abstract_regular_grid.h"

struct HashRegularGrid : public RegularGrid
{
    std::map<std::pair<int, int>, int> grid;

    HashRegularGrid(std::vector<Point>& _points, double _cellSize);
    void insertPoint(int pointIdx) override;
    bool searchPoint(Point &point) override;
};

#endif