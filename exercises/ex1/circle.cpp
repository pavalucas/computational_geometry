#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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
    bool operator== (const Point &o) const {
        return cmp_double(x, o.x) == 0 && cmp_double(y, o.y) == 0;
    }
};

double abs(const Point& p) {
    return sqrt(p * p);
}

Point norm(Point v) {
	return v / abs(v);
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

// Check if point p is inside circle c
bool inCircle(const Circle &c, const Point &p) {
	return cmp_double(abs(c.p - p), c.r) <= 0;
}

Circle getCircleGivenTwoPoints(const Point& p1, const Point& p2) {
	Point circleCenter = (p1 + p2) / 2.0;
	double circleRadius = euclideanDistance(p1, p2) / 2.0;
	return Circle(circleCenter, circleRadius);
}

// Count how many points are outside circle c
int countPointListOutsideCircle(const Circle& c, const std::vector<Point>& pointList) {
	int result = 0;
	for (auto& p : pointList) {
		if (!inCircle(c, p)) result++;
	}
	return result;
}

Circle heuristicMinCircle(const std::vector<Point>& pointList) {
    int n = (int) pointList.size();
	// base cases
	if (n == 0) return Circle();
	if (n == 1) return Circle(pointList[0], 0);

    // find two pairs of points: {p_xmin, p_xmax}, {p_ymin, pymax}
    Point pXMax = pointList[0], pXMin = pointList[0], pYMin = pointList[0], pYMax = pointList[0];
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
    std::vector<Point> furthestPointList{pXMin, pXMax, pYMin, pYMax};
    int iMax = 0, jMax = 0;
    double maxDistance = 0.0;
    for(int i = 0; i < 4; i++) {
		std::cout << furthestPointList[i].x << ", " << furthestPointList[i].y << '\n';
        for(int j = 0; j < 4; j++) {
            double currentDistance = euclideanDistance(furthestPointList[i], furthestPointList[j]);
            if(cmp_double(maxDistance, currentDistance) < 0) {
                maxDistance = currentDistance;
                iMax = i;
                jMax = j;
            }
        }
    }
	std::cout << furthestPointList[iMax].x << ", " << furthestPointList[iMax].y << '\n';
	std::cout << furthestPointList[jMax].x << ", " << furthestPointList[jMax].y << '\n';

	Circle resultCircle = getCircleGivenTwoPoints(furthestPointList[iMax], furthestPointList[jMax]);
	for (auto& point : pointList) {
		Point d = point - resultCircle.p;
		double dAbs = abs(d);
		Point dHat = norm(d);
		if (cmp_double(dAbs, resultCircle.r) > 0) {
			resultCircle.p = resultCircle.p + dHat * ((dAbs - resultCircle.r) / 2.0);
			resultCircle.r = (dAbs + resultCircle.r) / 2.0;
		}
	}
	return resultCircle;
}

Circle minCircleWithPoint(const std::vector<Point>& pointList, const Point& q) {
	int n = (int)pointList.size();
	// base case
	if (n == 0) return Circle();

	std::vector<Circle> circleList(n);
	circleList[0] = getCircleGivenTwoPoints(pointList[0], q);
	for (int i = 1; i < n; i++) {

	}
}

Circle deterministicMinCircle(const std::vector<Point>& pointList) {
	int n = (int)pointList.size();
	// base cases
	if (n == 0) return Circle();
	if (n == 1) return Circle(pointList[0], 0);

	std::vector<Point> shufflePointList = pointList;
	std::random_shuffle(shufflePointList.begin(), shufflePointList.end());
	std::vector<Circle> circleList(n);
	circleList[1] = getCircleGivenTwoPoints(pointList[0], pointList[1]);
	for (int i = 2; i < n; i++) {
		if (inCircle(circleList[i-1], pointList[i])) {
			circleList[i] = circleList[i - 1];
		}
		else {
		
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
	Circle heuristicCircle = heuristicMinCircle(pointList);
	std::cout << "Heuristic Circle center: " << heuristicCircle.p.x << ", " << heuristicCircle.p.y << '\n';
	std::cout << "Heuristic Circle radius: " << heuristicCircle.r << '\n';
	std::cout << "Points outside circle: " << countPointListOutsideCircle(heuristicCircle, pointList) << '\n';
    
    return 0;
}

