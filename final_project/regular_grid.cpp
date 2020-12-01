#include "regular_grid.h"


RegularGrid::RegularGrid(std::vector<Point>& _points, double _cellSize)
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
    gridSizeX = std::ceil((xmax-xmin)/cellSize);
    gridSizeY = std::ceil((ymax-ymin)/cellSize);

    // insert points
    for(int i=0; i<points.size(); i++) insertPoint(i);
}

std::pair<int, int> RegularGrid::getGridCoords(Point& point)
{
    int xIdx = std::floor((point.coord[0]-xmin)/cellSize);
    int yIdx = std::floor((point.coord[1]-ymin)/cellSize);
    return std::make_pair(xIdx, yIdx);
}

void RegularGrid::insertPoint(int pointIdx)
{
    auto& point = points[pointIdx];
    auto gridCoords = getGridCoords(point);
    if(grid.count(gridCoords))
    {
        // store the point in the "linked list" of that his cell
        int nextPoint = grid[gridCoords];
        grid[gridCoords] = pointIdx;
        cellPointsList[pointIdx] = nextPoint;
    }
    else
    {
        // start a new "linked list" on this point's cell
        grid[gridCoords] = pointIdx;
        cellPointsList[pointIdx] = -1;
    }
}

bool RegularGrid::searchPoint(Point &point)
{
    auto gridCoords = getGridCoords(point);
    if (!grid.count(gridCoords)) return false;
    
    int pointIdx = grid[gridCoords];
    if (points[pointIdx] == point) return true;

    while(cellPointsList[pointIdx] != -1)
    {
        pointIdx = cellPointsList[pointIdx];
        if (points[pointIdx] == point) return true;
    }
    return false;
}