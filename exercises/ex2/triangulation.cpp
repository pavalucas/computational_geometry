#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <queue>

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

// Return 1 if a->b->c is counterclockwise, -1 if it is clockwise and 0 otherwise
int ccw(Point p, Point q, Point r) {
    return cmp_double((q - p) % (r - p));
}

int getNextIndex(int i, int n, std::vector<bool>& activeVertices) {
	for (int j = 1; j < n; j++) {
		int index = (i + j) % n;
		if (activeVertices[index]) {
			return index;
		}
	}
    return -1;
}

int getLastIndex(int i, int n, std::vector<bool>& activeVertices) {
	for (int j = 1; j < n; j++) {
		int index = i - j;
		if (index < 0) index = n + (index % n);
		if (activeVertices[index]) {
			return index;
		}
	}
	return -1;
}

bool checkPointInsideTriangle(Point t1, Point t2, Point t3, Point p) {
    int ccw1 = ccw(t1, t2, p);
    int ccw2 = ccw(t2, t3, p);
    int ccw3 = ccw(t3, t1, p);
    return ccw1 >= 0 && ccw2 >= 0 && ccw3 >= 0;
}

bool checkDiagonal(const std::vector<Point>& poly, int index1, int index2, int index3, std::vector<bool>& activeVertices) {
    if(ccw(poly[index1], poly[index2], poly[index3]) <= 0) return false;
    int n = (int) poly.size();
    for(int i = 0; i < n; i++) {
        if(i != index1 && i != index2 && i != index3 && activeVertices[i] &&
            checkPointInsideTriangle(poly[index1], poly[index2], poly[index3], poly[i])) {
            return false;
        }
    }
    return true;
}

bool checkEar(const std::vector<Point>& poly, int index, std::vector<bool>& activeVertices) {
    int n = (int) poly.size();
    int lastIndex = getLastIndex(index, n, activeVertices), nextIndex = getNextIndex(index, n, activeVertices);
    return checkDiagonal(poly, lastIndex, index, nextIndex, activeVertices);
}

std::vector<std::pair<int, int>> earClippingTriangulation(const std::vector<Point>& poly) {
    std::vector<std::pair<int, int>> triangulation;
    int n = (int) poly.size();
	std::vector<bool> activeVertices(n, true);
    std::queue<int> earQueue;

    // determine which vertices are ears
    for(int i = 0; i < n; i++) {
        if(checkEar(poly, i, activeVertices)) {
            earQueue.emplace(i);
        }
    }

    // removing ears from poly
    int currentPolySize = n;
    while(currentPolySize > 3 && !earQueue.empty()) {
        int i = earQueue.front();
        earQueue.pop();
        if(!activeVertices[i] || !checkEar(poly, i, activeVertices)) continue;
		activeVertices[i] = false;
        currentPolySize--;
        int lastIndex = getLastIndex(i, n, activeVertices), nextIndex = getNextIndex(i, n, activeVertices);
        triangulation.emplace_back(std::make_pair(lastIndex, nextIndex));
        std::vector<int> updateIndexes{lastIndex, nextIndex};
        for(auto& updateIndex : updateIndexes) {
            if(checkEar(poly, updateIndex, activeVertices)) {
                earQueue.emplace(updateIndex);
            }
        }
    }
    return triangulation;
}

int main() {
    std::ifstream inFile;
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
        for(auto& p : triangulation) {
            std::cout << std::min(p.first, p.second) << " " << std::max(p.first, p.second) << '\n';
        }
        inFile.close();
    }

    return 0;
}