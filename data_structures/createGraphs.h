#ifndef SRC_CREATE_GRAPHS_H
#define SRC_CREATE_GRAPHS_H

#include "Graph.h"

using namespace std;

class createGraphs{
public:
    static Graph<int> graphFromFile(string fileName);

    static vector<int> findIsolatedNodesInGraph(Graph<int> g);
    static void emitDOTFile(string fname, Graph<int> g);
};

#endif
