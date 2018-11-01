#include <cstdlib>
#include "Edge.h"


#ifndef C_VERSION_NODE_H
#define C_VERSION_NODE_H



class Node {
private:
    int ID;
    Edge* edges;
    int e_size;
    int n_edges;
    int pq_Value;

public:
    Node(int ID);
    Node();
    int addEdge(int ID, int value);
    Edge* getEdges();
    int getID();
    int getN_Edges();
    int getPQ_Value();
    void setPQ_Value(int value);
};
#endif //C_VERSION_NODE_H





