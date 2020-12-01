#ifndef POINT_H
#define POINT_H

#include <random>
#include <fstream>

// Return -1 if a < b, 0 if a = b and 1 if a > b.
int cmp_double(double a, double b = 0, double eps = 1e-9);

struct Point
{
    double coord[2];
    Point() {}
    Point(double x, double y);
    bool operator==(const Point &o) const;

    static std::vector<Point> generateRandomPointList(int n, int stdDev);
    static void createPointCloudFile(int n, std::vector<Point> pointList);
};

#endif