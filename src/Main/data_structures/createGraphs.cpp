/**
* @file createGraphs.cpp
 * @brief Implementation of graph creation and manipulation functions
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#include "./createGraphs.h"

void populateGraphs(Graph<int> *g, string filename);
void populateEdges(Graph<int> *g, string filename);

/**
 * @brief Finds isolated nodes in a graph
 *
 * A node is considered isolated if it has no outgoing edges and no incoming edges.
 *
 * @param g The graph to search for isolated nodes
 * @return vector<int> A vector containing the IDs of all isolated nodes
 */
vector<int> createGraphs::findIsolatedNodesInGraph(Graph<int> g) {
    int i, id;
    vector<int> res;
    vector<int> rem;

    for (auto v: g.getVertexSet()) {
        if (v->getAdj().empty()) {
            res.push_back(v->getInfo());
        }
    }

    for (auto v: g.getVertexSet()) {
        for (auto &e: v->getAdj()) {
            auto w = e->getDest();
            auto it = std::find(res.begin(), res.end(), w->getInfo());
            if (it != res.end()) {
                rem.push_back(w->getInfo());
            }
        }
    }

    for (i = 0; i < rem.size(); i++) {
        id = rem[i];
        auto it = std::find(res.begin(), res.end(), id);
        if (it != res.end())
            res.erase(it);
    }
    return res;
}

/**
 * @brief Creates a graph from data files in the specified folder
 *
 * Expects two files in the folder:
 * - Loc1.csv containing vertex information
 * - Dist1.csv containing edge information
 *
 * @param folder The path to the folder containing the data files
 * @return Graph<int> The constructed graph
 */
Graph<int> createGraphs::graphFromFile(string folder) {
    Graph<int> g;
    string Location = folder + "/Loc1.csv";
    string Distance = folder + "/Dist1.csv";

    populateGraphs(&g, Location);
    populateEdges(&g, Distance);
    return g;
}

/**
 * @brief Populates a graph with vertices from a CSV file
 *
 * File format should be: location,id,code,parking
 *
 * @param g Pointer to the graph to populate
 * @param filename Path to the CSV file containing vertex data
 */
void populateGraphs(Graph<int> *g, string filename) {
    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    cout << "File opened successfully!" << endl;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        istringstream iss(line);
        string location, id_str, code, parking_str;
        if (getline(iss, location, ',') &&
            getline(iss, id_str, ',') &&
            getline(iss, code, ',') &&
            getline(iss, parking_str, ',')) {
            int id = stoi(id_str);
            int parking = stoi(parking_str);
            g->addVertex(id);
            g->findVertex(id)->setLocation(location);
            g->findVertex(id)->setCode(code);
            g->findVertex(id)->setParking(parking);
        }

    }

    file.close();
}

/**
 * @brief Populates a graph with edges from a CSV file
 *
 * File format should be: location1,location2,Driving,Walking
 *
 * @param g Pointer to the graph to populate
 * @param filename Path to the CSV file containing edge data
 */
void populateEdges(Graph<int> *g, string filename) {
    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    cout << "File opened successfully!" << endl;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        istringstream iss(line);
        string location1, location2, Driving, Walking;
        if (getline(iss, location1, ',') &&
            getline(iss, location2, ',') &&
            getline(iss, Driving, ',') &&
            getline(iss, Walking, ',')) {
                int driving = (Driving == "X") ? -1 : stoi(Driving);
                int walking = (Walking == "X") ? -1 : stoi(Walking);

                g->addBidirectionalEdge(g->findCode(location1)->getInfo(), g->findCode(location2)->getInfo(),
                    driving, walking);
            }
    }

    file.close();
}
// ------------------------------------------------------------
//
// ------------------------------------------------------------

/**
 * @brief Generates a file representing the graph structure
 *
 * Creates a file that represents the graph connections.
 * Isolated nodes are handled specially to maintain visibility.
 *
 * @param gname The name to use for the output file
 * @param g The graph to process
 */
void createGraphs::emitDOTFile(string gname, Graph<int> g) {
    ofstream g_dot_file;
    int idx;
    vector<int> rem;

    g_dot_file.open(gname + ".gv");
    g_dot_file << "digraph " << gname << "{\n";


    g_dot_file << "label     = \"Graph: " << gname << "\";\n";
    g_dot_file << "labelloc  = top;\n";
    g_dot_file << "labeljust = left;\n";
    g_dot_file << "fontname  = calibri;\n";
    g_dot_file << "fontsize  = 16;\n";
    g_dot_file << "\n";

    rem = findIsolatedNodesInGraph(g);

    for (auto v: g.getVertexSet()) {
        idx = v->getInfo();
        auto it = std::find(rem.begin(), rem.end(), idx);
        if (it != rem.end()) {
            g_dot_file << "  " << idx << " -> " << idx << " [color = \"white\"];\n";
        } else {
            for (auto &e: v->getAdj()) {
                auto w = e->getDest();
                g_dot_file << "  " << v->getInfo() << " -> " << w->getInfo() << ";";
                g_dot_file << "\n";
            }
        }
    }

    g_dot_file << "}\n";
    g_dot_file.close();
}


// ------------------------------------------------------------
