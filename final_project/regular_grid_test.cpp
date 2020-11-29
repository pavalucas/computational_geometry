#include <chrono>
#include <iostream>
#include "point.h"
#include "regular_grid.h"

const int STD_DEV = 1e2;

int main()
{
    srand(42);
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    std::vector<double> randomPointCloudSize{1e3, 5*1e3, 1e4, 5*1e4, 1e5, 5*1e5, 1e6, 5*1e6};
    for (int i : randomPointCloudSize)
    {
        std::cout << "n = " << i << '\n';
        std::vector<Point> pointList = Point::generateRandomPointList(i, STD_DEV);
        
        begin = std::chrono::steady_clock::now();
        RegularGrid grid(pointList, STD_DEV/1000.0);
        end = std::chrono::steady_clock::now();
        
        std::cout << "Construct grid time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]\n";

        int numPointInList = 0;
        int numPointNotInList = 0;
        // begin = std::chrono::steady_clock::now();
        // for (int i = 0; i < pointList.size(); i += 2)
        // {
        //     Point modifiedPoint = pointList[i];
        //     modifiedPoint.coord[0] += 0.5;
        //     modifiedPoint.coord[1] += 0.5;
        //     if(searchPoint(root, pointList[i])) numPointInList++;
        //     else numPointNotInList++;
            
        //     if(searchPoint(root, modifiedPoint)) numPointInList++;
        //     else numPointNotInList++; 
        // }
        // end = std::chrono::steady_clock::now();
        // std::cout << "Search points = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]\n";
        // std::cout << "Number of points in list = " << numPointInList << '\n';
        // std::cout << "Number of points not in list = " << numPointNotInList << '\n';

    }
    return 0;
}