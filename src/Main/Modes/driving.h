/**
* @file driving.h
 * @brief Header file for driving and walking route calculations
 *
 * @details Implements Dijkstra's algorithm with dual modes (driving/walking)
 * for urban navigation system. Handles route planning with different
 * constraints for each transportation mode.
 */

#ifndef DRIVING_H
#define DRIVING_H

#include "../data_structures/Graph.h"

using namespace std;

/**
 * @brief Relaxation function for driving routes
 * @tparam T Type of vertex information
 * @param edge Pointer to the edge being relaxed
 * @return True if relaxation was successful (shorter path found)
 *
 * @details Checks:
 * - If edge is drivable (drivingTime != -1)
 * - If destination node is available
 * - If origin node is available
 * - If a shorter path is found through this edge
 */
template <class T>
bool relaxdriving(Edge<T> *edge) { //for the driving part of the path

    if (edge->getDrivingTime() == -1) {return false;} //can't drive on that edge

    Vertex<T> *v = edge->getDest();
    if (v->getAvailable() == -1) {return false;}

    if (edge->getOrig()->getAvailable() == -1) {return false;}

    if (edge->getOrig()->getDist() + edge->getDrivingTime() < edge->getDest()->getDist()) { // we have found a better way to reach v
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getDrivingTime()); // d[v] = d[u] + w(u,v)
        edge->getDest()->setPath(edge); // set the predecessor of v to u; in this case the edge from u to v
        return true;
    }
    return false;
}

/**
 * @brief Relaxation function for walking routes
 * @tparam T Type of vertex information
 * @param edge Pointer to the edge being relaxed
 * @return True if relaxation was successful (shorter path found)
 *
 * @details Checks:
 * - If edge is walkable (walkingTime != -1)
 * - If destination node is available
 * - If origin node is available
 * - If a shorter path is found through this edge
 */
template <class T>
bool relaxwalking(Edge<T> *edge) { //for the walking part of the path

    if (edge->getWalkingTime() == -1) {return false;} //can't drive on that edge

    Vertex<T> *v = edge->getDest();
    if (v->getAvailable() == -1) {return false;}

    if (edge->getOrig()->getAvailable() == -1) {return false;}

    if (edge->getOrig()->getDist() + edge->getWalkingTime() < edge->getDest()->getDist()) { // we have found a better way to reach v
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->getWalkingTime()); // d[v] = d[u] + w(u,v)
        edge->getDest()->setPath(edge); // set the predecessor of v to u; in this case the edge from u to v
        return true;
    }
    return false;
}

/**
 * @brief Dijkstra's algorithm implementation with mode switching
 * @tparam T Type of vertex information
 * @param g Pointer to the graph object
 * @param source ID of the source vertex
 *
 * @details Computes shortest paths using:
 * - Driving mode when switchwalking = false
 * - Walking mode when switchwalking = true
 * Uses a priority queue for efficient path calculation
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
            if (!e->getDest()->isVisited()) {
                auto oldDist = e->getDest()->getDist();
                if (g->switchwalking) {
                    if (relaxwalking(e)) {
                        if (oldDist == INF) {
                            q.insert(e->getDest());
                        }
                        else {
                            q.decreaseKey(e->getDest());
                        }
                    }
                } else {
                    if (relaxdriving(e)) {
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
 * Validates that the path starts at the correct origin.
 */
template <class T>
static std::vector<T> getPath(Graph<T> * g, const int &origin, const int &dest) {
    std::vector<T> res;
    auto v = g->findVertex(dest);
    if (v == nullptr || v->getDist() == INF) { // missing or disconnected
        return res;
    }
    if (g->findVertex(origin)->getDist() == INF) { // missing or disconnected
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
#endif //DRIVING_H
