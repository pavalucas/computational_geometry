#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

// Return -1 if a < b, 0 if a = b and 1 if a > b.
int cmp_double(double a, double b = 0, double eps = 1e-9) {
    return a + eps > b ? b + eps > a ? 0 : 1 : -1;
}

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    Point operator+ (const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator+ (const double &o) const { return Point(x + o, y + o); }
    Point operator- (const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator* (const double &o) const { return Point(x * o, y * o); }
    Point operator/ (const double &o) const { return Point(x / o, y / o); }
    double operator* (const Point &o) const { return x * o.x + y * o.y; }
    double operator% (const Point &o) const { return x * o.y - o.x * y; }
    bool operator== (const Point &o) const {
        return cmp_double(x, o.x) == 0 && cmp_double(y, o.y) == 0;
    }
    bool operator< (const Point &o) const {
        return x != o.x ? x < o.x : y < o.y;
    }
};

// Return 1 if a->b->c is counterclockwise, -1 if it is clockwise and 0 otherwise
int ccw(Point p, Point q, Point r) {
    return cmp_double((q - p) % (r - p));
}

std::vector<Point> convexHull(std::vector<Point>& pointList) {
    int n = (int) pointList.size();
    std::vector<Point> result;
    sort(pointList.begin(), pointList.end());
    result.emplace_back(pointList[0]);
    for (int i = 1; i < n; i++) {
        // check if current point is above (0, n-1) segment
        if (i != n-1 && ccw(pointList[0], pointList[n-1], pointList[i]) >= 0) continue;
        while (result.size() > 1 &&
               ccw(result[result.size()-2], result[result.size()-1], pointList[i]) <= 0) {
            result.pop_back();
        }
        result.emplace_back(pointList[i]);
    }
    for (int i = n-2, lim = result.size(); i >= 0; i--) {]
        // check if current point is below (0, n-1) segment
        if (i != 0 && ccw(pointList[n-1], pointList[0], pointList[i]) >= 0) continue;
        while (result.size() > lim &&
               ccw(result[result.size()-2], result[result.size()-1], pointList[i]) <= 0) {
            result.pop_back();
        }
        result.emplace_back(pointList[i]);
    }
    return result;
}

int main() {
    std::ifstream inFile;

    inFile.open("nuvem1.txt");
    if (!inFile) {
        std::cout << "Unable to open file\n";
        return -1; // terminate with error
    }

    double x, y;
    std::vector<Point> pointList;
    while(inFile >> x >> y) {
        pointList.emplace_back(Point(x, y));
    }

    std::vector<Point> result = convexHull(pointList);
    for(auto& p : result) {
        std::cout << p.x << ", " << p.y << '\n';
    }
    return 0;
}