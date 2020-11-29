#pragma once

#include <random>

// Return -1 if a < b, 0 if a = b and 1 if a > b.
int cmp_double(double a, double b = 0, double eps = 1e-9)
{
    return a + eps > b ? b + eps > a ? 0 : 1 : -1;
}

struct Point
{
    double coord[2];
    Point() {}
    Point(double x, double y)
    {
        coord[0] = x;
        coord[1] = y;
    }

    bool operator==(const Point &o) const
    {
        return cmp_double(coord[0], o.coord[0]) == 0 && cmp_double(coord[1], o.coord[1]) == 0;
    }

    static std::vector<Point> generateRandomPointList(int n, int stdDev)
    {
        std::vector<Point> result(n);
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(0.0, stdDev);
        for (int i = 0; i < n; i++)
        {
            // points with coordinates between 1 and maxCoord
            result[i] = Point(distribution(generator), distribution(generator));
        }
        return result;
    }
};
