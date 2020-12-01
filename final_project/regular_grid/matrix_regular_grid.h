#ifndef MATRIX_REGULAR_GRID_H
#define MATRIX_REGULAR_GRID_h

#include "abstract_regular_grid.h"

struct MatrixRegularGrid : public RegularGrid
{
    std::vector<std::vector<int>> grid;

    MatrixRegularGrid(std::vector<Point>& _points, double _cellSize);
    void insertPoint(int pointIdx) override;
    bool searchPoint(Point &point) override;
};

#endif