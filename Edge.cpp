#include "Edge.h"
Edge::Edge(int ID_From, int ID_To, int value) {

    this->ID_From=ID_From;
    this->ID_To=ID_To;
    this->value=value;
}

int Edge::getID_From(){
    return this->ID_From;
}

int Edge::getID_To(){
    return this->ID_To;
}
int Edge::getValue(){
    return this->value;
}