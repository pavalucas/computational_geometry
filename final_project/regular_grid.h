#ifndef REGULAR_GRID_H
#define REGULAR_GRID_H

#include <map>
#include <vector>
#include <cmath>
#include <limits>
#include <utility>
#include "point.h"

struct RegularGrid
{
private:
    std::vector<Point> points;
    std::map<std::pair<int, int>, int> grid;
    long long int gridSizeX, gridSizeY;
    double cellSize, xmin, ymin, xmax, ymax;
    std::vector<int> cellPointsList;

    std::pair<int, int> getGridCoords(Point& point);
    void insertPoint(int pointIdx);

public:
    RegularGrid(std::vector<Point>& _points, double _cellSize);
    bool searchPoint(Point &point);
};

#endif