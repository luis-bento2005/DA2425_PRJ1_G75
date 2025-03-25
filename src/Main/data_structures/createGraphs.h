/**
* @file createGraphs.h
 * @brief Header file for graph creation utilities
 */

#ifndef SRC_CREATE_GRAPHS_H
#define SRC_CREATE_GRAPHS_H

#include "Graph.h"

using namespace std;

/**
 * @class createGraphs
 * @brief Provides static methods for graph creation and analysis
 *
 * This class contains utility functions for:
 * - Creating graphs from files
 * - Analyzing graph properties
 * - Outputting graph representations
 */
class createGraphs{
public:
    /**
     * @brief Creates a graph from data files in the specified folder
     * @param fileName Path to the folder containing the data files
     * @return Graph<int> The constructed graph
     */
    static Graph<int> graphFromFile(string fileName);

    /**
     * @brief Finds isolated nodes in a graph (nodes with no connections)
     * @param g The graph to analyze
     * @return vector<int> List of IDs of isolated nodes
     */
    static vector<int> findIsolatedNodesInGraph(Graph<int> g);

    /**
     * @brief Generates a file representing the graph structure
     * @param fname The name to use for the output file
     * @param g The graph to process
     */
    static void emitDOTFile(string fname, Graph<int> g);
};

#endif
