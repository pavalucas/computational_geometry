#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <tuple>
#include <set>
#include <random>
#include <chrono>

const int STD_DEV = 100;

// Return -1 if a < b, 0 if a = b and 1 if a > b.
int cmp_double(double a, double b = 0, double eps = 1e-9) {
    return a + eps > b ? b + eps > a ? 0 : 1 : -1;
}

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    Point operator+ (const Point& o) const { return Point(x + o.x, y + o.y); }
    Point operator+ (const double& o) const { return Point(x + o, y + o); }
    Point operator- (const Point& o) const { return Point(x - o.x, y - o.y); }
    Point operator* (const double& o) const { return Point(x * o, y * o); }
    Point operator/ (const double& o) const { return Point(x / o, y / o); }
    double operator* (const Point& o) const { return x * o.x + y * o.y; }
    double operator% (const Point& o) const { return x * o.y - o.x * y; }
    bool operator== (const Point& o) const {
        return cmp_double(x, o.x) == 0 && cmp_double(y, o.y) == 0;
    }
    bool operator< (const Point& o) const {
        return x != o.x ? x < o.x : y < o.y;
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

// Check if point p is inside circle c
bool inCircle(const Circle& c, const Point& p) {
    return cmp_double(euclideanDistance(c.p, p), c.r) <= 0;
}

Circle circumCenter(const Point& p, const Point& q, const Point& r) {
    Point a = p - r, b = q - r, c = Point(a * (p + r)/2, b * (q + r)/2);
    Point circleCenter = Point(c % Point(a.y, b.y), Point(a.x, b.x) % c) / (a % b);
    double circleRadius = euclideanDistance(circleCenter, p);
    return Circle(circleCenter, circleRadius);
}


struct Edge {
    Point u, v;
    bool toRemove;
    Edge() {}
    Edge(Point u, Point v) : u(u), v(v), toRemove(false) {}
    bool operator== (const Edge& o) const {
        return (u == o.u && v == o.v) || (v == o.u && u == o.v);
    }
    bool operator< (const Edge& o) const {
        return !(u == o.u) ? u < o.u : v < o.v;
    }
};

struct Triangle {
    Point a, b, c;
    bool toRemove;
    Triangle() {}
    Triangle(Point a, Point b, Point c) : a(a), b(b), c(c), toRemove(false) {}
    bool circumCircleContainsPoint(const Point& p) {
        Circle circle = circumCenter(a, b, c);
        return inCircle(circle, p);
    }
    bool isVertexEqualPoint(Point& p) {
        return a == p || b == p || c == p;
    }
};

class Delaunay {
    std::vector<Point> points;

    public:

        Delaunay(std::vector<Point>& pointList) {
            points = pointList;
        }

        std::vector<Triangle> triangulate() {
            // initialize result
            std::vector<Triangle> triangles;

            // Determine enclosing triangle
            Triangle enclosingTriangle = getEnclosingTriangle();
            triangles.emplace_back(enclosingTriangle);

            for(auto& point : points) {

                std::set<Edge> edgesForNewTriangles;
                
                for(auto& triangle : triangles) {
                    if(triangle.circumCircleContainsPoint(point)) {
                        // mark triangle to remove later
                        triangle.toRemove = true;

                        // add edges to create new triangles
                        Edge ab = Edge(triangle.a, triangle.b);
                        Edge ac = Edge(triangle.a, triangle.c);
                        Edge bc = Edge(triangle.b, triangle.c);
                        insertOrEraseEdgeFromSet(ab, edgesForNewTriangles);
                        insertOrEraseEdgeFromSet(ac, edgesForNewTriangles);
                        insertOrEraseEdgeFromSet(bc, edgesForNewTriangles);
                    }
                }
                // remove triangles which contains current point
                triangles.erase(std::remove_if(triangles.begin(), triangles.end(), [](Triangle& t) {
                    return t.toRemove;
                }), triangles.end());

                // add new triangles using edgesForNewTriangles
                for(auto& edge : edgesForNewTriangles) {
                    triangles.emplace_back(Triangle(edge.u, edge.v, point));
                }
            }
            // remove triangles which have vertices equal to a vertex from enclosing triangle
            triangles.erase(std::remove_if(triangles.begin(), triangles.end(), [&](Triangle& t) {
                return t.isVertexEqualPoint(enclosingTriangle.a) || t.isVertexEqualPoint(enclosingTriangle.b) ||
                       t.isVertexEqualPoint(enclosingTriangle.c);
            }), triangles.end());

            return triangles;
        }

    private:
        Triangle getEnclosingTriangle() {
            double minX = points[0].x;
            double maxX = minX;
            double minY = points[0].y;
            double maxY = minY;

            for(auto& p : points) {
                minX = std::min(minX, p.x);
                maxX = std::max(maxX, p.x);
                minY = std::min(minY, p.y);
                maxY = std::max(maxY, p.y);
            }

            double halfSide = std::max(maxX - minX, maxY - minY);
            double midX = (maxX + minX) / 2.0;
            double midY = (maxY + minY) / 2.0;
            Point a = Point(midX - 20.0 * halfSide, midY - 20.0 * halfSide);
            Point b = Point(midX, midY + 20.0 * halfSide);
            Point c = Point(midX + 20.0 * halfSide, midY);
            return Triangle(a, b, c);
        }

        void insertOrEraseEdgeFromSet(const Edge& e, std::set<Edge>& edgesSet) {
            // if edge is already in set remove it, otherwise add it
            if(edgesSet.count(e)) {
                edgesSet.erase(e);
            } else {
                edgesSet.emplace(e);
            }
        }

};

// Auxiliary methods
std::vector<std::tuple<int, int, int>> getTriangulationIndexes(const std::vector<Point>& pointList, const std::vector<Triangle>& triangles) {
    std::vector<std::tuple<int, int, int>> result;
    std::map<Point, int> pointToIndex;
    for(int i = 0; i < pointList.size(); i++) {
        pointToIndex.emplace(std::make_pair(pointList[i], i));
    }
    for(auto& t : triangles) {
        int indexA = pointToIndex[t.a];
        int indexB = pointToIndex[t.b];
        int indexC = pointToIndex[t.c];
        result.emplace_back(std::make_tuple(indexA, indexB, indexC));
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

void createTriangulationFile(const std::string& name, const std::vector<std::tuple<int, int, int>>& trianglesIndexes) {
    std::ofstream outFile;
    outFile.open(name);
    for(auto& p : trianglesIndexes) {
        outFile << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p) << '\n';
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
        inFile.close();

        Delaunay delaunay = Delaunay(pointList);
        std::vector<Triangle> triangleList = delaunay.triangulate();
        std::vector<std::tuple<int, int, int>> trianglesIndexes = getTriangulationIndexes(pointList, triangleList);
        std::string name = "delaunay" + std::to_string(fileNum) + ".txt";
        std::cout << "Delaunay triangulation for " << file << ":" << '\n';
        for(auto& p : trianglesIndexes) {
            std::cout << std::get<0>(p) << " " << std::get<1>(p) << " " << std::get<2>(p) << '\n';
        }
        std::cout << '\n';
        
        createTriangulationFile(name, trianglesIndexes);
        fileNum++;
    }

    std::cout << "Testing with different number of points, randomly generated" << '\n';
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    std::vector<double> randomPointCloudSize{1e2, 5*1e2, 1e3, 5*1e3, 1e4};
    for(int i : randomPointCloudSize) {
        std::cout << "n = " << i << '\n';
        std::vector<Point> pointList = generateRandomPointList(i, STD_DEV);

        Delaunay delaunay = Delaunay(pointList);
        begin = std::chrono::steady_clock::now();
        std::vector<Triangle> triangleList = delaunay.triangulate();
        end = std::chrono::steady_clock::now();
        std::cout << "Delaunay triangulation execution time = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << '\n';

        createPointCloudFile(i, pointList);

        std::vector<std::tuple<int, int, int>> trianglesIndexes = getTriangulationIndexes(pointList, triangleList);
        std::string name = "delaunayTri" + std::to_string(i) + ".txt";
        createTriangulationFile(name, trianglesIndexes);

        std::cout << "\n\n";
    }

    return 0;
}