#include <limits.h>
#include <cstdio>
#include "Node.h"


#define VERBOSE false

Node::Node() : ID(0), n_edges(0), e_size(4), edges(), pq_Value(INT_MAX) {}

Node::Node(int ID) {
    this->ID = ID;
    this->edges = (Edge *) malloc(4 * sizeof(Edge));
    e_size = 4;
    n_edges = 0;
    pq_Value = INT_MAX;
}

int Node::addEdge(int ID, int value) {
    for (int i = 0; i < this->n_edges; i++) {
        if (this->edges[i].getID_To() == ID) {
            //fprintf(stdout, "!!!Edge already present (%d, %d)\n",this->ID,ID);
            return -1;
        }

    }

    if (n_edges == e_size) {
        this->edges = (Edge *) realloc(edges, 4 * e_size * sizeof(Edge));
        e_size *= 4;
    }

    edges[n_edges] = Edge(this->ID, ID, value);
    this->n_edges++;

    if(VERBOSE)
        fprintf(stdout, "\nCreate edge between %d and %d, value=%d", this->ID, ID, value);

    return n_edges;
}

int Node::getID() {
    return this->ID;
}

Edge *Node::getEdges() {
    return this->edges;
}

int Node::getN_Edges() {
    return this->n_edges;
}

int Node::getPQ_Value() {
    return this->pq_Value;
}

void Node::setPQ_Value(int value) {
    this->pq_Value = value;
}