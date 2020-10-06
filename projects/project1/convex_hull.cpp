#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <random>

const int MAXN = 1e6;
const int STD_DEV = 1e2;

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

std::vector<Point> convexHullGrahamScan(const std::vector<Point>& pointListOriginal) {
    std::vector<Point> pointList = pointListOriginal;
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
    for (int i = n-2, lim = result.size(); i >= 0; i--) {
        // check if current point is below (0, n-1) segment
        if (i != 0 && ccw(pointList[n-1], pointList[0], pointList[i]) >= 0) continue;
        while (result.size() > lim &&
               ccw(result[result.size()-2], result[result.size()-1], pointList[i]) <= 0) {
            result.pop_back();
        }
        result.emplace_back(pointList[i]);
    }
    result.pop_back();
    return result;
}

std::vector<int> getConvexHulIndexes(std::vector<Point> pointList, std::vector<Point> convexHull) {
    std::vector<int> result;
    std::map<Point, int> pointToIndex;
    for(int i = 0; i < pointList.size(); i++) {
        pointToIndex.emplace(std::make_pair(pointList[i], i));
    }
    for(auto& p : convexHull) {
        result.emplace_back(pointToIndex[p]);
    }
    return result;
}

std::vector<Point> generateRandomPointList(int n, int stdDev) {
    std::vector<Point> result(n);
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, stdDev);
    for(int i = 0; i < n; i++) {
        // points with coordinates between 1 and maxCoord
        result[i] = Point(distribution(generator), distribution(generator));
    }
    return result;
}

void createPointCloudFile(int n, std::vector<Point> pointList) {
    std::ofstream outFile;
    outFile.open("pointCloud" + std::to_string(n) + ".txt");
    for(auto& p : pointList) {
        outFile << p.x << " " << p.y << '\n';
    }
    outFile.close();
}

void createConvexHullFile(std::string name, std::vector<int> convexHullIndexes) {
    std::ofstream outFile;
    outFile.open(name);
    for(auto& p : convexHullIndexes) {
        outFile << p << '\n';
    }
    outFile.close();
}

int main() {
    srand (42);
    std::ifstream inFile;
    std::vector<std::string> pointFiles{"nuvem1.txt", "nuvem2.txt"};

    std::cout << "Testing with given point files" << '\n';
    int fileNum = 1;
    for(auto& file : pointFiles) {
        inFile.open(file);
        if (!inFile) {
            std::cout << "Unable to open file\n";
            return -1; // terminate with error
        }

        double x, y;
        std::vector<Point> pointList;
        while(inFile >> x >> y) {
            pointList.emplace_back(Point(x, y));
        }

        std::vector<Point> convexHullPoints = convexHullGrahamScan(pointList);
        std::vector<int> convexHullIndexes = getConvexHulIndexes(pointList, convexHullPoints);
        std::string name = "fecho" + std::to_string(fileNum) + ".txt";
        createConvexHullFile(name, convexHullIndexes);
        std::cout << "Convex hull indexes for " << file << ":" << '\n';
        for(auto& p : convexHullIndexes) {
            std::cout << p << '\n';
        }
        std::cout << '\n';
        inFile.close();
        fileNum++;
    }



    std::cout << "Testing with different number of points, randomly generated" << '\n';
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    std::vector<double> randomPointCloudSize{1e3, 5*1e3, 1e4, 5*1e4, 1e5, 5*1e5, 1e6, 5*1e6};
    for(int i : randomPointCloudSize) {
        std::cout << "n = " << i << '\n';
        std::vector<Point> pointList = generateRandomPointList(i, STD_DEV);

        begin = std::chrono::steady_clock::now();
        std::vector<Point> convexHullPoints = convexHullGrahamScan(pointList);
        end = std::chrono::steady_clock::now();
        std::cout << "Convex hull Graham Scan execution time = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << '\n';

        std::vector<Point> tmpPointList = pointList;
        begin = std::chrono::steady_clock::now();
        sort(tmpPointList.begin(), tmpPointList.end());
        end = std::chrono::steady_clock::now();
        std::cout << "Sort point list execution time = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << '\n';

        createPointCloudFile(i, pointList);

        std::vector<int> convexHullIndexes = getConvexHulIndexes(pointList, convexHullPoints);
        std::string name = "convexHull" + std::to_string(i) + ".txt";
        createConvexHullFile(name, convexHullIndexes);

        std::cout << "\n\n";
    }
    return 0;
}