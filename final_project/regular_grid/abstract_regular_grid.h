#ifndef REGULAR_GRID_H
#define REGULAR_GRID_H

#include <cmath>
#include <limits>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
#include "../point.h"


struct RegularGrid
{
    std::vector<Point> points;
    long long int gridSizeX, gridSizeY;
    double cellSize, xmin, ymin, xmax, ymax;
    std::vector<int> cellPointsList;

    RegularGrid(std::vector<Point>& _points, double _cellSize)
    {
        cellSize = _cellSize;
        points = _points;
        cellPointsList = std::vector<int>(points.size(), -1);

        // find bounding box
        xmin = ymin = std::numeric_limits<double>::max();
        xmax = ymax = std::numeric_limits<double>::min();
        for(auto& p : points)
        {
            xmin = std::min(xmin, p.coord[0]);
            xmax = std::max(xmax, p.coord[0]);
            ymin = std::min(ymin, p.coord[1]);
            ymax = std::max(ymax, p.coord[1]);
        }

        // saving width and height
        gridSizeX = std::floor((xmax-xmin)/cellSize) + 1;
        gridSizeY = std::floor((ymax-ymin)/cellSize) + 1;
    }

    std::pair<int, int> getGridCoords(Point& point)
    {
        int xIdx = std::floor((point.coord[0]-xmin)/cellSize);
        int yIdx = std::floor((point.coord[1]-ymin)/cellSize);
        return std::make_pair(xIdx, yIdx);
    }

    virtual void insertPoint(int pointIdx) = 0;
    virtual bool searchPoint(Point &point) = 0;
};

#endif