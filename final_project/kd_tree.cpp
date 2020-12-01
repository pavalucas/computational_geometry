#include "kd_tree.h"

KDNode *insertPoint(KDNode *node, Point p, int cutDim)
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
bool searchPoint(KDNode *node, Point p, int cutDim)
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
