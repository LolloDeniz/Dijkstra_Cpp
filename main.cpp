#include <iostream>
#include <math.h>
#include <limits.h>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <fstream>
#include "Node.h"
#include "PQ.h"

#define VERBOSE false
#define N_NODES 15000
#define N_EDGES 2.5*N_NODES

int dijkstra(int startID, Node *nodes, int endID, int n_nodes);
Node* readJSON(char* path, int n_nodes);

int main() {

    //while(1){
        using namespace std::chrono;

        //reading from json
        //Node* nodes;
        //nodes=readJSON("../edges.json", N_NODES);
        //int startID=8191, endID=11548;

        milliseconds tic, toc;

        //setvbuf(stdout, NULL, _IONBF, 0);
        //setvbuf(stderr, NULL, _IONBF, 0);


        //init of random function
        srand((unsigned) time(nullptr));

        Node nodes[N_NODES];

        //generation of nodes
        //for ensuring the complete connection of the graph, the nodes are a tree
        Node::generateNodes(nodes, N_NODES);

        //random generation of edges
        Node::generateEdges(nodes,N_NODES, N_EDGES);

        //random generation of start and end
        int startID = (rand() % N_NODES);
        int endID;
        while ((endID = (rand() % N_NODES)) == startID);


        tic=duration_cast< milliseconds >(system_clock::now().time_since_epoch());

        //let's find the way!
        dijkstra(startID, nodes, endID, N_NODES);

        toc=duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        duration<double, std::milli> time_span = toc - tic;


        //diagnostics tests
        Node::testEdges(nodes, N_NODES);

        fprintf(stdout, "\n\nElapsed time: %f ms", time_span.count());
    //}

    return 0;
}

int dijkstra(int startID, Node *nodes, int endID, int n_nodes) {

    Node *v, *w;
    Edge *t;

    //init heap-based priority queue
    PQ pq = PQ();
    pq.PQinit(n_nodes);

    //init of parent array
    int *st;
    st = (int *) malloc(n_nodes * sizeof(int));

    if (startID >= n_nodes || startID < 0) return -1;
    if (endID>= n_nodes || endID < 0) return -1;

    //initial insertion of nodes in PQ
    for (int i = 0; i < n_nodes; i++) {
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
        for (int i = 0; i < n_nodes; i++) {
            fprintf(stdout, "parent of %d id %d \n", i, st[i]);
        }
        fprintf(stdout, "min distances from %d\n", startID);
        for (int i = 0; i < n_nodes; i++) {
            fprintf(stdout, "%d : %d metres\n", i, nodes[i].getPQ_Value());
        }
    }
        fprintf(stdout, "\n\nPath from %d to %d:\n", startID, endID);
        int j = endID;
        while (st[j] != j) {
            fprintf(stdout, "%d->", j);
            j = st[j];
        }
        fprintf(stdout, "%d", startID);
        fprintf(stdout, "\nWeight=%d", nodes[endID].getPQ_Value());

return 1;
}

Node* readJSON(char* path, int n_nodes){

    int ID1, ID2, distance;
    char toIgnore[20];
    FILE* fp;

    char line[50];

    //open file
    fp=fopen(path, "r");
    if(fp==NULL){
        fprintf(stderr, "Unable to open %s", path);
        return NULL;
    }

    //init array of nodes
    Node* nodes=(Node*) malloc(n_nodes* sizeof(Node));
    for(int i=0;i<n_nodes;i++) nodes[i]=Node(i);


    //read the json line by line
    //on the side is commented the expected result of the read

    fgets(line, 50, fp);   //[

      while((fgets(line, 50, fp))&& line[1]!=']'){  //{
        fgets(line, 50, fp);    //ID1=...
        sscanf(line, "%s %d,", toIgnore, &ID1);

        fgets(line, 50, fp);    //ID2=...
        sscanf(line, "%s %d,", toIgnore, &ID2);

        fgets(line, 50, fp);    //DISTANCE=...
        sscanf(line, "%s %d,", toIgnore, &distance);

        fgets(line, 50, fp);    //}

        nodes[ID1].addEdge(ID2, distance);
        nodes[ID2].addEdge(ID1, distance);
        //fprintf(stdout,"%d %d %d\n", ID1, ID2, distance); //debug
    }
    return nodes;
}
