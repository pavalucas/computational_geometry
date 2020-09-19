#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <unordered_set>

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
};

// Return true if a->b->c is counterclockwise and false otherwise
bool ccw(Point p, Point q, Point r) {
    return cmp_double((q - p) % (r - p)) > 0;
}

int getNextIndex(int i, int n) {
    int nextIndex = i+1;
    if(nextIndex == n) nextIndex = 0;
    return nextIndex;
}

int getLastIndex(int i, int n) {
    int lastIndex = i-1;
    if(lastIndex == -1) lastIndex = n-1;
    return lastIndex;
}

bool checkPointInsideTriangle(Point t1, Point t2, Point t3, Point p) {
    bool ccw1 = ccw(t1, t2, p);
    bool ccw2 = ccw(t2, t3, p);
    bool ccw3 = ccw(t3, t1, p);
    return ccw1 && ccw2 && ccw3;
}

bool checkDiagonal(const std::vector<Point>& poly, int index1, int index2, int index3) {
    int n = (int) poly.size();
    for(int i = 0; i < n; i++) {
        if(i != index1 && i != index2, i != index3 &&
            checkPointInsideTriangle(poly[index1], poly[index2], poly[index3], poly[i])) {
            return false;
        }
    }
    return true;
}

bool checkEar(const std::vector<Point>& poly, int index) {
    int n = (int) poly.size();
    int lastIndex = getLastIndex(index, n), nextIndex = getNextIndex(index, n);
    return checkDiagonal(poly, lastIndex, index, nextIndex);
}

std::vector<std::pair<int, int>> earClippingTriangulation(const std::vector<Point>& poly) {
    std::vector<std::pair<int, int>> triangulation;
    int n = (int) poly.size();
    std::unordered_set<int> earSet;
    // determine if each vertex is a ear
    for(int i = 0; i < n; i++) {
        if(checkEar(poly, i)) {
            earSet.emplace(i);
        }
    }

    // removing ears
    int currentPolySize = n;
    while(currentPolySize > 3 && !earSet.empty()) {
        int i = *(earSet.begin());
        earSet.erase(earSet.begin());
        currentPolySize--;
        int lastIndex = getLastIndex(i, n), nextIndex = getNextIndex(i, n);
        triangulation.emplace_back(std::make_pair(lastIndex, nextIndex));
        // TODO lidar com o problema do vertice ser removido do poligono
        std::vector<int> updateIndexes{lastIndex, nextIndex};
        for(auto& updateIndex : updateIndexes) {
            if(checkEar(poly, updateIndex) && !earSet.count(updateIndex)) {
            earSet.emplace(updateIndex);
            } else if(!checkEar(poly, updateIndex) && earSet.count(updateIndex)) {
                earSet.erase(updateIndex);
            }
        }
    }
    return triangulation;
}

int main() {
    std::ifstream inFile;
    // std::vector<std::string> polygonFiles{"polygon1.txt", "polygon2.txt"};
    std::vector<std::string> polygonFiles{"polygon2.txt"};

    for(auto& polygonFile : polygonFiles) {
        inFile.open(polygonFile);
        if (!inFile) {
            std::cout << "Unable to open file\n";
            return -1; // terminate with error
        }
        std::vector<Point> poly;
        double x, y;
        while(inFile >> x >> y) {
            poly.emplace_back(Point(x, y));
        }
        std::vector<std::pair<int, int>> triangulation = earClippingTriangulation(poly);
        std::cout << "Polygon: " << polygonFile << '\n';
        for(auto& p : triangulation) {
            std::cout << p.first << " " << p.second << '\n';
        }
        std::cout << '\n';
        inFile.close();
    }

    return 0;
}