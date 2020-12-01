#include "hash_regular_grid.h"

HashRegularGrid::HashRegularGrid(std::vector<Point>& _points, double _cellSize)
    : RegularGrid(_points, _cellSize)
{
    // insert points
    for(int i=0; i<points.size(); i++) insertPoint(i);
}

void HashRegularGrid::insertPoint(int pointIdx)
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

bool HashRegularGrid::searchPoint(Point &point)
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
