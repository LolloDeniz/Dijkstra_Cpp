
#ifndef C_VERSION_EDGE_H
#define C_VERSION_EDGE_H


class Edge {

private:
    int ID_From;
    int ID_To;
    int value;

public:
    Edge(int ID_From, int ID_To, int value);
    int getID_From();
    int getID_To();
    int getValue();
};


#endif //C_VERSION_EDGE_H
