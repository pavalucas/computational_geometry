#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "point.h"

const int STD_DEV = 1e2;

struct KDNode
{
    Point data;
    KDNode *left;
    KDNode *right;

    KDNode() {}
    KDNode(Point data) : data(data), left(nullptr), right(nullptr) {}
};

// Main methods

KDNode *insertPoint(KDNode *node, Point p, int cutDim = 0)
{
    if (node == nullptr)
    {
        node = new KDNode(p);
    }
    else if (p == node->data)
    {
        std::cout << "ERROR: inserting node that already exists\n";
        node = nullptr;
    }
    // cutDim = 0 -> cutting dimension is x
    // cutDim = 1 -> cutting dimension is y
    else if (p.coord[cutDim] < node->data.coord[cutDim])
    {
        node->left = insertPoint(node->left, p, 1 - cutDim);
    }
    else
    {
        node->right = insertPoint(node->right, p, 1 - cutDim);
    }
    return node;
}
bool searchPoint(KDNode *node, Point p, int cutDim = 0)
{
    bool result = false;
    if (node == nullptr)
    {
        result = false;
    }
    else if (p == node->data)
    {
        result = true;
    }
    // cutDim = 0 -> cutting dimension is x
    // cutDim = 1 -> cutting dimension is y
    else if (p.coord[cutDim] < node->data.coord[cutDim])
    {
        result = searchPoint(node->left, p, 1 - cutDim);
    }
    else
    {
        result = searchPoint(node->right, p, 1 - cutDim);
    }
    return result;
}


int main()
{
    srand(42);
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    std::vector<double> randomPointCloudSize{1e3, 5*1e3, 1e4, 5*1e4, 1e5, 5*1e5, 1e6, 5*1e6};
    for (int i : randomPointCloudSize)
    {
        std::cout << "n = " << i << '\n';
        std::vector<Point> pointList = Point::generateRandomPointList(i, STD_DEV);
        KDNode *root = nullptr;
        begin = std::chrono::steady_clock::now();
        for (auto &p : pointList)
        {
            root = insertPoint(root, p);
        }
        end = std::chrono::steady_clock::now();
        std::cout << "Construct tree time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]\n";

        int numPointInList = 0;
        int numPointNotInList = 0;
        begin = std::chrono::steady_clock::now();
        for (int i = 0; i < pointList.size(); i += 2)
        {
            Point modifiedPoint = pointList[i];
            modifiedPoint.coord[0] += 0.5;
            modifiedPoint.coord[1] += 0.5;
            if(searchPoint(root, pointList[i])) numPointInList++;
            else numPointNotInList++;
            
            if(searchPoint(root, modifiedPoint)) numPointInList++;
            else numPointNotInList++; 
        }
        end = std::chrono::steady_clock::now();
        std::cout << "Search points = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]\n";
        std::cout << "Number of points in list = " << numPointInList << '\n';
        std::cout << "Number of points not in list = " << numPointNotInList << '\n';

    }
    return 0;
}