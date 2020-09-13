#include <iostream>
#include <fstream>
#include <vector>

// Return -1 if a < b, 0 if a = b and 1 if a > b.
int cmp_double(double a, double b = 0, double eps = 1e-9) {
    return a + eps > b ? b + eps > a ? 0 : 1 : -1;
}

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    Point operator+ (const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator- (const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator* (const double &o) const { return Point(x * o, y * o); }
    Point operator/ (const double &o) const { return Point(x / o, y / o); }
    double operator* (const Point &o) const { return x * o.x + y * o.y; }
    bool operator== (const Point &o) const {
        return cmp_double(x, o.x) == 0 && cmp_double(y, o.y) == 0;
    }
};

double abs(const Point& p) {
    return sqrt(p * p);
}

double euclideanDistance(const Point& p, const Point& q) {
    return abs(p - q);
}

struct Circle {
    Point p;
    double r;
    Circle() {}
    Circle(Point p, double r) : p(p), r(r) {}
};



Circle heuristic_min_circle(vector<Point>& pointList) {
    int n = (int) pointList.size();
    if(n == 0) return Circle();

    // find two pairs of points: {p_xmin, p_xmax}, {p_ymin, pymax}
    Point pXMax = poinList[0], pXMin = pointList[0], pYMin = pointList[0], pYMax = pointList[0];
    for(auto& point : pointList) {
        if(cmp_double(pXMin.x, point.x) > 0) {
            pXMin = point;
        }
        if(cmp_double(pXMax.x, point.x) < 0) {
            pXMax = point;
        }
        if(cmp_double(pYMin.y, point.y) > 0) {
            pYMin = point;
        }
        if(cmp_double(pYMin.y, point.y) < 0) {
            pYMax = point;
        }
    }

    // get furthest pair: p_i, p_j
    vector<Point> furthestPointList{pXMin, pXMax, pYMin, pYMax};
    int iMax = 0, jMax = 0;
    double maxDistance = 0.0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            double currentDistance = euclideanDistance(furthestPointList[i], furthestPointList[j]);
            if(cmp_double(maxDistance, currentDistance) < 0) {
                maxDistance = currentDistance;
                iMax = i;
                jMax = j;
            }
        }
    }
}



int main() {
    std::ifstream inFile;
    
    inFile.open("points.txt");
    if (!inFile) {
        std::cout << "Unable to open file\n";
        return -1; // terminate with error
    }

    double x, y;
    std::vector<Point> pointList;
    while(inFile >> x >> y) {
        pointList.emplace_back(Point(x, y));
    }
    std::cout << pointList.size() << '\n';

    
    return 0;
}

