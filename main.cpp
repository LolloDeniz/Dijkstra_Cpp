#include <iostream>
#include "Node.h"
#include "PQ.h"
#include <math.h>
#include <limits.h>
#include <cstdlib>
#include <time.h>
#include <chrono>

#define VERBOSE false
#define N_NODES 15000
#define N_EDGES 2.5*N_NODES
#define MAX_VALUE 30
#define STD_VALUE MAX_VALUE/2

int generateNodes(Node *nodes);

int _generateNodes(Node *nodes, Node *current, int h, int treeHeight);

int generateEdges(Node *nodes);

int testEdges(Node *nodes);

int dijkstra(int startID, Node *nodes, int endID);

int n = 0; //progressive ID

int main() {

    //while(1){
        n=0;
        using namespace std::chrono;
        milliseconds tic, toc;

        //setvbuf(stdout, NULL, _IONBF, 0);
        //setvbuf(stderr, NULL, _IONBF, 0);


        //init of random function
        srand((unsigned) time(0));

        Node nodes[N_NODES];
        //generation of nodes
        //for ensuring the complete connection of the graph, the nodes are a tree
        generateNodes(nodes);

        //random generation of edges
        generateEdges(nodes);

        //random generation of start and end
        int stardID = (rand() % N_NODES);
        int endID;
        while ((endID = (rand() % N_NODES)) == stardID);


        tic=duration_cast< milliseconds >(system_clock::now().time_since_epoch());

        //let's find the way!
        dijkstra(stardID, nodes, endID);

        toc=duration_cast< milliseconds >(system_clock::now().time_since_epoch());

        duration<double, std::milli> time_span = toc - tic;



        //diagnostics tests
        testEdges(nodes);

        fprintf(stdout, "\n\nElapsed time: %f ms", time_span.count());
    //}

    return 0;
}

int generateNodes(Node *nodes) {

    int treeHeight = (int) floor(log2(N_NODES));
    nodes[0] = Node(0);

    _generateNodes(nodes, &nodes[0], 1, treeHeight);
}

int _generateNodes(Node *nodes, Node *current, int h, int treeHeight) {

    //left
    if (n < N_NODES - 1 && h <= treeHeight) {
        n++;
        nodes[n] = Node(n);

        current->addEdge(n, STD_VALUE);
        nodes[n].addEdge(current->getID(), STD_VALUE);

        if(VERBOSE){
            for (int i = 0; i < h; i++) {
                fprintf(stdout, "|\t");
            }
            fprintf(stdout, "left son of %d created (ID=%d)\n", current->getID(), n);
            fflush(stdout);
        }

        _generateNodes(nodes, &nodes[n], h + 1, treeHeight);
    }

    //right
    if (n < N_NODES - 1 && h <= treeHeight) {
        n++;
        nodes[n] = Node(n);

        current->addEdge(n, STD_VALUE);
        nodes[n].addEdge(current->getID(), STD_VALUE);

        if(VERBOSE){
            for (int i = 0; i < h; i++) {
                fprintf(stdout, "|\t");
            }
            fprintf(stdout, "right son of %d created (ID=%d)\n", current->getID(), n);
            fflush(stdout);
        }

        _generateNodes(nodes, &nodes[n], h + 1, treeHeight);
    }

}

int testEdges(Node *nodes) {
    int e = 0;
    int sum_wh = 0;

    for (int i = 0; i < N_NODES; i++) {
        if(VERBOSE)
            printf("\nNODE %d CONNECTED TO ", i);
        for (int j = 0; j < nodes[i].getN_Edges(); j++) {
            if(VERBOSE)
                printf("%d ", nodes[i].getEdges()[j].getID_To());

            e++;
            sum_wh += nodes[i].getEdges()[j].getValue();
        }
    }

    fprintf(stdout, "\nNumber of nodes: %d", N_NODES);
    fprintf(stdout, "\nNumber of edges: %d", (e / 2));
    fprintf(stdout, "\nAverage edges per node: %f", ((e / 2) / (float) N_NODES));

}

int generateEdges(Node *nodes) {

    int ID1, ID2, value;


    //  |
    // \|/ I have to subtract the number of already existing edges (equals to number of nodes)

    for (int i = N_NODES; i < N_EDGES; i++) {
        ID1 = (rand() % N_NODES);
        while ((ID2 = (rand() % N_NODES)) == ID1);
        value = rand() % MAX_VALUE;

        nodes[ID1].addEdge(ID2, value);
        nodes[ID2].addEdge(ID1, value);
    }

    fprintf(stdout, "\n\n");

}

int dijkstra(int startID, Node *nodes, int endID) {

    Node *v, *w;
    Edge *t;


    //init heap-based priority queue
    PQ pq = PQ();
    pq.PQinit(N_NODES);

    //init of parent array
    int *st;
    st = (int *) malloc(N_NODES * sizeof(int));

    if (startID >= N_NODES || startID < 0) return -1;
    if (endID>= N_NODES || endID < 0) return -1;

    //initial insertion of nodes in PQ
    for (int i = 0; i < N_NODES; i++) {
        st[i] = -1;
        pq.PQinsert(&nodes[i]);
        //initial value of distance is INT_MAX
    }

    //the distance from himself is 0
    nodes[startID].setPQ_Value(0);
    //the father of the first node is himself
    st[startID] = startID;

    //update the priority queue with the distance value
    pq.PQchange(startID, &(nodes[startID]));

    while (!pq.PQempty()) {
        if ((v = pq.PQextractMax())->getPQ_Value() != INT_MAX) {
            for (int i = 0; i < v->getN_Edges(); i++) {    //for every edge of the node
                t = &(v->getEdges()[i]);
                w = &nodes[t->getID_To()];
                if (v->getPQ_Value() + t->getValue() <
                    w->getPQ_Value()) {  //if the distance of the node + edge distance is less than distance of the connected node
                    w->setPQ_Value(
                            v->getPQ_Value() + t->getValue());     //the distance of the connected node is updated
                    pq.PQchange(pq.PQfindPos(w),
                                w);                    //update of PQ (I need to find the position of node for the PQChange)
                    st[w->getID()] = v->getID();                          //update the father array
                }
            }
        }
    }

    if(VERBOSE){
        for (int i = 0; i < N_NODES; i++) {
            fprintf(stdout, "parent of %d id %d \n", i, st[i]);
        }
        fprintf(stdout, "min distances from %d\n", startID);
        for (int i = 0; i < N_NODES; i++) {
            fprintf(stdout, "%d : %d metres\n", i, nodes[i].getPQ_Value());
        }

        fprintf(stdout, "\n\nPath from %d to %d:\n", startID, endID);
        int j = endID;
        while (st[j] != j) {
            fprintf(stdout, "%d->", j);
            j = st[j];
        }
        fprintf(stdout, "%d", startID);
        fprintf(stdout, "\nWeight=%d", nodes[endID].getPQ_Value());
    }
return 1;
}

