#ifndef KD_TREE_H
#define KD_TREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "point.h"

struct KDNode
{
    Point data;
    KDNode *left;
    KDNode *right;

    KDNode() {}
    KDNode(Point data) : data(data), left(nullptr), right(nullptr) {}
};

KDNode *insertPoint(KDNode *node, Point p, int cutDim = 0);
bool searchPoint(KDNode *node, Point p, int cutDim = 0);

#endif