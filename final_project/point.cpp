#include "point.h"

int cmp_double(double a, double b, double eps)
{
    return a + eps > b ? b + eps > a ? 0 : 1 : -1;
}

Point::Point(double x, double y)
{
    coord[0] = x;
    coord[1] = y;
}

bool Point::operator==(const Point &o) const
{
    return cmp_double(coord[0], o.coord[0]) == 0 && cmp_double(coord[1], o.coord[1]) == 0;
}

std::vector<Point> Point::generateRandomPointList(int n, int stdDev)
{
    std::vector<Point> result(n);
    std::default_random_engine generator;
    // std::normal_distribution<double> distribution(0.0, stdDev);
    std::uniform_real_distribution<double> distribution(-400, 400);
    for (int i = 0; i < n; i++)
    {
        // points with coordinates between 1 and maxCoord
        result[i] = Point(distribution(generator), distribution(generator));
    }
    return result;
}

void Point::createPointCloudFile(int n, std::vector<Point> pointList) {
    std::ofstream outFile;
    outFile.open("uniformPointCloud" + std::to_string(n) + ".txt");
    for(auto& p : pointList) {
        outFile << p.coord[0] << " " << p.coord[1] << '\n';
    }
    outFile.close();
}