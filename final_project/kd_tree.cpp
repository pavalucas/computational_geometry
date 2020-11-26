#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

// Return -1 if a < b, 0 if a = b and 1 if a > b.
int cmp_double(double a, double b = 0, double eps = 1e-9) {
    return a + eps > b ? b + eps > a ? 0 : 1 : -1;
}

struct Point {
    double coord[2];
    Point() {}
    Point(double x, double y) {
        coord[0] = x;
        coord[1] = y;
    }
    bool operator== (const Point &o) const {
        return cmp_double(coord[0], o.coord[0]) == 0 && cmp_double(coord[1], o.coord[1]) == 0;
    }
};

struct KDNode {
    Point data;
    // cutDim = 0 -> cutting dimension is x
    // cutDim = 1 -> cutting dimension is y
    int cutDim;
    KDNode* left;
    KDNode* right;

    KDNode() {}
    KDNode(Point data) : data(data), left(nullptr), right(nullptr), cutDim(0) {}
};

// Main methods

KDNode* insertPoint(KDNode* node, Point p, int cutDim = 0) {
    if(node == nullptr) {
        node = new KDNode(p);
    } else if(p == node->data) {
        std::cout << "ERROR: inserting node that already exists\n";
        node = nullptr;
    } else if(p.coord[cutDim] < node->data.coord[cutDim]) { 
        node->left = insertPoint(node->left, p, 1 - cutDim);
    } else {
        node->right = insertPoint(node->right, p, 1 - cutDim);
    }
    return node;
}

// Auxiliary methods

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

int main() {
    std::vector<Point> pointList = generateRandomPointList(5, 100);
    KDNode* root = nullptr;
    for(auto& p : pointList) {
        root = insertPoint(root, p);
    }
    return 0;
}