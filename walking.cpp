#include "data_structures/Graph.h"
#include "data_structures/MutablePriorityQueue.h"
#include <list>
using namespace std;


template <class T>
bool relax(Edge<T> *edge) { // d[u] + w(u,v) < d[v]
    if (edge->getOrig()->getDist() + edge->time_driving() < edge->getDest()->getDist()) { // we have found a better way to reach v
        edge->getDest()->setDist(edge->getOrig()->getDist() + edge->time_walking); // d[v] = d[u] + w(u,v)
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

template <class T>
double getCost(Graph<T> * g, const int &dest) {return g->findVertex(dest)->getDist();}