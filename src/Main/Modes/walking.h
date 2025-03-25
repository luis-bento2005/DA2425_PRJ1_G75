/**
* @file walking.h
 * @brief Header file for walking route calculations
 *
 * @details Implements Dijkstra's algorithm specialized for walking routes
 * in the urban navigation system.
 */

#ifndef WALKING_H
#define WALKING_H

#include "../data_structures/Graph.h"

using namespace std;

/**
 * @brief Relaxation function for Dijkstra's algorithm (walking mode)
 * @tparam T Type of vertex information
 * @param edge Pointer to the edge being relaxed
 * @return True if relaxation was successful, false otherwise
 *
 * @details Updates the shortest path estimate if a shorter path is found
 * through the given edge. Uses driving time as the weight metric.
 */
template <class T>
bool relax(Edge<T> *edge) { // d[u] + w(u,v) < d[v]
    if (edge->getOrig()->getDist() + edge->getDrivingTime()< edge->getDest()->getDist()) { // we have found a better way to reach v
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getDrivingTime()); // d[v] = d[u] + w(u,v)
        edge->getDest()->setPath(edge); // set the predecessor of v to u; in this case the edge from u to v
        return true;
    }
    return false;
}

/**
 * @brief Dijkstra's algorithm implementation for walking routes
 * @tparam T Type of vertex information
 * @param g Pointer to the graph object
 * @param source ID of the source vertex
 *
 * @details Computes shortest paths from source to all other vertices
 * using driving times as edge weights. Maintains a priority queue
 * for efficient extraction of the next vertex to process.
 */

template <class T>
void dijkstra(Graph<T> * g, const int &source) {

    // Initialize the vertices
    for(auto v : g->getVertexSet()) {
        v->setDist(INF);
        v->setPath(nullptr);
    }
    auto s = g->findVertex(source);
    s->setDist(0);

    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while( ! q.empty() ) {
        auto v = q.extractMin();
        for(auto e : v->getAdj()) {
            auto oldDist = e->getDest()->getDist();
            if (relax(e)) {
                if (oldDist == INF) {
                    q.insert(e->getDest());
                }
                else {
                    q.decreaseKey(e->getDest());
                }
            }
        }
    }
}

/**
 * @brief Reconstructs the shortest path from origin to destination
 * @tparam T Type of vertex information
 * @param g Pointer to the graph object
 * @param origin ID of the origin vertex
 * @param dest ID of the destination vertex
 * @return Vector containing the sequence of vertex IDs in the path
 *
 * @details Traces back the path from destination to origin using
 * predecessor links, then reverses it to get the correct order.
 */
template <class T>
static std::vector<T> getPath(Graph<T> * g, const int &origin, const int &dest) {
    std::vector<T> res;
    auto v = g->findVertex(dest);
    if (v == nullptr || v->getDist() == INF) { // missing or disconnected
        return res;
    }
    res.push_back(v->getInfo());
    while(v->getPath() != nullptr) {
        v = v->getPath()->getOrig();
        res.push_back(v->getInfo());
    }
    reverse(res.begin(), res.end());
    if(res.empty() || res[0] != origin) {
        std::cout << "Origin not found!!" << std::endl;
    }
    return res;
}

/**
 * @brief Gets the cost (distance) to reach a destination vertex
 * @tparam T Type of vertex information
 * @param g Pointer to the graph object
 * @param dest ID of the destination vertex
 * @return The shortest path distance to the destination
 */
template <class T>
double getCost(Graph<T> * g, const int &dest) {return g->findVertex(dest)->getDist();}
#endif //WALKING_H
