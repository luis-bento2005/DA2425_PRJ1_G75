// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "../data_structures/MutablePriorityQueue.h" // not needed for now

template <class T>
class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    Vertex(T in);
    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue

    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    void setInfo(T info);
    void setVisited(bool visited);
    void setProcessing(bool processing);

    int getLow() const;
    void setLow(int value);
    int getNum() const;
    void setNum(int value);

    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge<T> *path);
    Edge<T> * addEdge(Vertex<T> *dest, int Driving, int Walking);
    bool removeEdge(T in);
    void removeOutgoingEdges();

    /**
    * @brief Sets the physical location description of the vertex
    * @param Location The human-readable location name
    */
    void setLocation(std::string Location);

    /**
    * @brief Sets the unique identifier code for the vertex
    * @param Code The alphanumeric code identifying this location
    */
    void setCode(std::string Code);

    /**
    * @brief Gets the vertex's unique identifier code
    * @return The code string that identifies this vertex
    * @see setCode()
    */
    std::string getCode() const;

    /**
    * @brief Sets the parking availability at this vertex
    * @param Parking true if parking is available at this location, false otherwise
    */
    void setParking(bool Parking);

    /**
     * @brief Sets the node's availability status for routing algorithms
     * @param Available The availability state:
     *        - -1 = Node blocked (avoided in routes)
     *        -  0 = Normally available (default state)
     *        -  1 = Specially included (required in routes)
     */
    void setAvailable(int Available);

    /**
    * @brief Gets the current routing availability state
    * @return The availability state:
    *        - -1 = Blocked
    *        -  0 = Normal
    *        -  1 = Required
    * @see setAvailable()
    */
    int getAvailable() const;

    /**
    * @brief Quick check for parking availability
    * @return true if parking is available, false otherwise
    * @see setParking()
    */
    bool getParking() const {
        return this->parking;
    }

    friend class MutablePriorityQueue<Vertex>;
protected:
    T info;                // info node
    std::vector<Edge<T> *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    int low = -1, num = -1; // used by SCC Tarjan
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);

    /**
    * @var std::string Vertex::Location
    * @brief Human-readable description of the vertex's physical location
    */
    std::string Location;

    /**
    * @var std::string Vertex::Code
    * @brief Unique alphanumeric identifier for the vertex
    */
    std::string Code;

    /**
    * @var bool Vertex::parking
    * @brief Parking availability flag
    * @details
    * - true: Vertex is a valid parking location
    * - false: Vertex cannot be used for parking transitions
    */
    bool parking = false;

    /**
    * @var int Vertex::available
    * @brief Routing availability state (-1, 0, or 1)
    * @details State meanings:
    * - -1 = Node blocked (excluded from all routes)
    * -  0 = Normally available (default state)
    * -  1 = Required inclusion (forces routing through node)
    */
    int available = 0; // -1,0,1
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, int Driving, int Walking);

    Vertex<T> * getDest() const;

    /**
    * @brief Gets the driving time between vertices
    * @return Driving time in minutes
    */
    int getDrivingTime() const;

    /**
    * @brief Gets the walking time between vertices
    * @return Walking time in minutes
    */
    int getWalkingTime() const;
    bool isSelected() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge<T> *reverse);
    void setFlow(double flow);
protected:
    Vertex<T> * dest; // destination vertex

    /**
    * @var int Edge::driving
    * @brief Driving time between vertices in minutes
    */
    int driving;

    /**
    * @var int Edge::walking
    * @brief Walking time between vertices in minutes
    */
    int walking; //
    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex<T> *orig;
    Edge<T> *reverse = nullptr;

    double flow; // for flow-related problems
};

/********************** Graph  ****************************/

template <class T>
class Graph {
public:
    ~Graph();
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex<T> *findVertex(const T &in) const;

    /**
    * @brief Finds a vertex by its unique location code
    * @param in The alphanumeric code to search for
    * @return Pointer to the vertex if found, nullptr otherwise
    */
    Vertex<T> *findCode(const std::string &in) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const T &in);
    bool removeVertex(const T &in);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, int Driving, int Walking);
    bool removeEdge(const T &source, const T &dest);
    bool addBidirectionalEdge(const T &sourc, const T &dest, int Driving, int Walking);

    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    /**
    * @var int Graph::includenodevar
    * @brief Special node inclusion flag for restricted routing
    * @details
    * - Default: -1 (no forced inclusion)
    * - When set to a vertex ID, routes must pass through this node
    */
    int includenodevar = -1;

    /**
    * @var bool Graph::switchwalking
    * @brief Transportation mode selector for routing algorithms
    * @details
    * - false: Use driving times (default mode)
    * - true: Use walking times
    */
    bool switchwalking = false; //false means driving true means walking

protected:
    std::vector<Vertex<T> *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    /*
     * Finds the index of the vertex with a given content.
     */
    int findVertexIdx(const T &in) const;
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);


/************************* Vertex  **************************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}
/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge driving time and walking time.
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, int driving, int walking) {
    auto newEdge = new Edge<T>(this, d, driving, walking);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
template <class T>
void Vertex<T>::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}
//New Code

/**
 * @brief Sets the human-readable location description of the vertex
 * @param Location Descriptive string
 */
template<class T>
void Vertex<T>::setLocation(std::string Location) {
    this->Location = Location;
}

/**
 * @brief Sets the unique alphanumeric identifier for the vertex
 * @param Code Unique code string
 * @warning Must be unique across all vertices in the graph
 */
template<class T>
void Vertex<T>::setCode(std::string Code) {
    this->Code = Code;
}

/**
 * @brief Retrieves the vertex's unique identifier code
 * @return The vertex's code string
 */
template<class T>
std::string Vertex<T>::getCode() const{
    return this->Code;
}

/**
 * @brief Sets parking availability at this vertex
 * @param Parking true if vertex is a valid parking location
*/
template<class T>
void Vertex<T>::setParking(bool Parking) {
    this->parking = Parking;
}

/**
 * @brief Sets vertex availability for routing algorithms
 * @param Available Node state:
 *        - -1 = Blocked (excluded from routes)
 *        -  0 = Normal availability
 *        -  1 = Required inclusion
*/
template<class T>
void Vertex<T>::setAvailable(int Available) {
    this->available = Available;
}

/**
 * @brief Gets current routing availability state
 * @return Current availability value (-1, 0, or 1)
 */
template<class T>
int Vertex<T>::getAvailable() const {
    return this->available;
}

//
template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
int Vertex<T>::getLow() const {
    return this->low;
}

template <class T>
void Vertex<T>::setLow(int value) {
    this->low = value;
}

template <class T>
int Vertex<T>::getNum() const {
    return this->num;
}

template <class T>
void Vertex<T>::setNum(int value) {
    this->num = value;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, int Driving, int Walking): orig(orig), dest(dest), driving(Driving), walking(Walking) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

/**
 * @brief Retrieves the driving time for this edge
 * @return Driving time in minutes
 */
template<class T>
int Edge<T>::getDrivingTime() const {
    return this->driving;
}

/**
 * @brief Retrieves the walking time for this edge
 * @return Walking time in minutes
*/
template<class T>
int Edge<T>::getWalkingTime() const {
    return this->walking;
}

template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/**
 * @brief Finds a vertex by its unique location code
 * @param in The alphanumeric code to search for
 * @return Pointer to the vertex if found, nullptr if not found
 */
template<class T>
Vertex<T> * Graph<T>::findCode(const std::string &in) const {
    for (auto v : vertexSet)
        if (v->getCode() == in)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getInfo() == in) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getInfo());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, int Driving, int Walking) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, Driving, Walking);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, int Driving, int Walking) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, Driving, Walking);
    auto e2 = v2->addEdge(v1, Driving, Walking);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}


inline void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

inline void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

template <class T>
Graph<T>::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */