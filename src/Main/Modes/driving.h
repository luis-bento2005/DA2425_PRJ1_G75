#ifndef DRIVING_H
#define DRIVING_H

#include "../data_structures/Graph.h"

using namespace std;

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

template <class T>
bool relaxwalking(Edge<T> *edge) { //for the walking part of the path

    if (edge->getDrivingTime() == -1) {return false;} //can't drive on that edge

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

template <class T>
double getCost(Graph<T> * g, const int &dest) {return g->findVertex(dest)->getDist();}
#endif //DRIVING_H
