#include "matrix_regular_grid.h"


MatrixRegularGrid::MatrixRegularGrid(std::vector<Point>& _points, double _cellSize)
: RegularGrid(_points, _cellSize)
{
    grid = std::vector<std::vector<int>>(gridSizeX, std::vector<int>(gridSizeY, -1));
    // insert points
    for(int i=0; i<points.size(); i++) insertPoint(i);
}

void MatrixRegularGrid::insertPoint(int pointIdx)
{
    auto& point = points[pointIdx];
    auto gridCoords = getGridCoords(point);
    auto& gridCell = grid[gridCoords.first][gridCoords.second];
    if(gridCell != -1)
    {
        // store the point in the "linked list" of that his cell
        int nextPoint = gridCell;
        gridCell = pointIdx;
        cellPointsList[pointIdx] = nextPoint;
    }
    else
    {
        // start a new "linked list" on this point's cell
        gridCell = pointIdx;
        cellPointsList[pointIdx] = -1;
    }
}

bool MatrixRegularGrid::searchPoint(Point &point)
{
    auto gridCoords = getGridCoords(point);
    if(gridCoords.first < 0 || gridCoords.second < 0 ||
        gridCoords.first >= gridSizeX || gridCoords.second >= gridSizeY)
        return false;
    
    auto& gridCell = grid[gridCoords.first][gridCoords.second];
    if (gridCell == -1) return false;
    
    int pointIdx = gridCell;
    if (points[pointIdx] == point) return true;

    while(cellPointsList[pointIdx] != -1)
    {
        pointIdx = cellPointsList[pointIdx];
        if (points[pointIdx] == point) return true;
    }
    return false;
}
