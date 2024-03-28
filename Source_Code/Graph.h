// Original code by Gonçalo Leão
// Updated by DA 2023/2024 Team

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "VertexType.h"

template <class T>
class Edge;

/************************* Vertex  **************************/

template <class T>
class Vertex {
public:
    Vertex(T in, VertexType type_);
    T getInfo() const;
    VertexType getType() const;
    std::vector<Edge<T> *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge<T> *getPath() const;
    std::vector<Edge<T> *> getIncoming() const;

    void setInfo(T info);
    void setVisited(bool visited);
    void setProcesssing(bool processing);
    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge<T> *path);
    Edge<T> * addEdge(Vertex<T> *dest, double w);
    bool removeEdge(T in);
    void removeOutgoingEdges();
    bool operator<(Vertex<T> & vertex) const;

protected:
    T info;                // info node
    std::vector<Edge<T> *> adj;  // outgoing edges
    VertexType type;


    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming; // incoming edges

    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double w);

    Vertex<T> * getDest() const;
    double getWeight() const;
    bool isSelected() const;
    Vertex<T> * getOrig() const;
    Edge<T> *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge<T> *reverse);
    void setFlow(double flow);
    void setWeight(double weight);

protected:
    Vertex<T> * dest; // destination vertex
    double weight; // edge weight, can also be used for capacity

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
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const T &in, VertexType type);
    bool removeVertex(const T &in);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const T &sourc, const T &dest, double w);
    bool removeEdge(const T &source, const T &dest);
    bool addBidirectionalEdge(const T &sourc, const T &dest, double w);

    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    std:: vector<T> dfs() const;
    std:: vector<T> dfs(const T & source) const;
    void dfsVisit(Vertex<T> *v,  std::vector<T> & res) const;
    std::vector<T> bfs(const T & source) const;

    bool isDAG() const;
    bool dfsIsDAG(Vertex<T> *v) const;
    std::vector<T> topsort() const;
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
Vertex<T>::Vertex(T in, VertexType type_): info(in), type(type_) {}

/**
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param d Vertex of destination.
 * @param w Weight of the edge
 * @return Pointer to the edges created
 */
template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    d->setIndegree(d->getIndegree() + 1);
    return newEdge;
}

/**
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Complexity: O(E^2) where E is the number of outgoing edges of the source vertex / incoming edges of the destination vertex.
 * @tparam T Type to the class
 * @param in Information of the destination edge
 * @return true if successful, and false if such edge does not exist.
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

/**
 * Auxiliary function to remove an outgoing edge of a vertex.
 * Complexity: O(E^2) where E is the number of outgoing edges of the source vertex / incoming edges of the destination vertex.
 * @tparam T Type of the class
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

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

/**
 * Gets the vertex's information.
 * Complexity: O(1)
 * @tparam T Type fo the class
 * @return Vertex's information
 */
template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

/**
 * Gets the vertex's type.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Vertex's type.
 */
template <class T>
VertexType Vertex<T>::getType() const{
    return this->type;
}

/**
 * Gets the vertex's outgoing edges list.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Vertex's outgoing edges list.
 */
template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

/**
 * Gets the vertex's visited state.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Vertex's visited state
 */
template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

/**
 * Gets the vertex's processing state.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Vertex's processing state
 */
template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

/**
 * Gets the vertex's indegree.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Vertex's indegree.
 */
template <class T>
unsigned int Vertex<T>::getIndegree() const {
    return this->indegree;
}

/**
 * Gets the vertex's distance from a given vertex.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Vertex's distance
 */
template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

/**
 * Gets the edge corresponding to a path.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Edge corresponding to a path.
 */
template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

/**
 * Gets the vertex's incoming edge list.
 *  * Complexity: O(1)
 * @tparam T Type of the class
 * @return Vertex's incoming edge list.
 */
template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

/**
 * Sets the vertex's information.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param in New information for the vertex.
 */
template <class T>
void Vertex<T>::setInfo(T in) {
    this->info = in;
}

/**
 * Sets the vertex's visited state.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param visited New visited state for the vertex.
 */
template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

/**
 * Sets the vertex's processing state.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param processing New processing state for the vertex.
 */
template <class T>
void Vertex<T>::setProcesssing(bool processing) {
    this->processing = processing;
}

/**
 * Sets the vertex's indegree.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param indegree New indegree for the vertex.
 */
template <class T>
void Vertex<T>::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

/**
 * Sets the vertex's distance to another vertex.
 * Complexity: O(1)
 * @tparam T Type fo the class.
 * @param dist New distance value.
 */
template <class T>
void Vertex<T>::setDist(double dist) {
    this->dist = dist;
}

/**
 * Sets the vertex's path.
 * Complexity: O(1)
 * @tparam T Type fo the class.
 * @param path New path.
 */
template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

/**
 * Deletes a given edge from the incoming edges list.
 * Complexity: O(E) where E is the number of incoming edges.
 * @tparam T Type fo the class.
 * @param edge edge we want to delete.
 */
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
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double w): orig(orig), dest(dest), weight(w) {}

/**
 * Gets the edge's destination.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Destination Vertex
 */
template <class T>
Vertex<T> * Edge<T>::getDest() const {
    return this->dest;
}

/**
 * Gets the edge weight.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Weight value
 */
template <class T>
double Edge<T>::getWeight() const {
    return this->weight;
}

/**
 * Gets the origin of the edge
 * @tparam T Type of the class
 * @return Origin vertex
 */
template <class T>
Vertex<T> * Edge<T>::getOrig() const {
    return this->orig;
}

/**
 * Gets the reverse edge.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Reverse edge
 */
template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return this->reverse;
}

/**
 * Gets the selected status.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return Returns the selected status
 */
template <class T>
bool Edge<T>::isSelected() const {
    return this->selected;
}

/**
 * Gets the edge flow.
 * Complexity: O(1)
 * @tparam T type of th class
 * @return Edge flow
 */
template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

/**
 * Sets the selected status.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param selected New selected status
 */
template <class T>
void Edge<T>::setSelected(bool selected) {
    this->selected = selected;
}

/**
 * Sets the reverse edge.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param reverse New reverse edge
 */
template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    this->reverse = reverse;
}

/**
 * Sets the flow value.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param flow New flow value
 */
template <class T>
void Edge<T>::setFlow(double flow) {
    this->flow = flow;
}

/**
 * Sets the weight value.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @param weight New weight value
 */
template <class T>
void Edge<T>::setWeight(double weight) {
    this->weight = weight;
}

/********************** Graph  ****************************/

/**
 * Gets the number of vertexes.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return  Number of Vertexes
 */
template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

/**
 * Gets the vector with the vertexes.
 * Complexity: O(1)
 * @tparam T Type of the class
 * @return  vector with the vertexes
 */
template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/**
 * Auxiliary function to find a vertex with a given content.
 * Complexity: O(v) where v is the number of vertexes
 * @param in Info of the vertex to find.
 * @return Pointer to the vertex found or nullptr if the vertex doesn't exists.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in)
            return v;
    return nullptr;
}

/**
 * Finds the index of the vertex with a given content.
 * Complexity: O(v) where v is the number of vertexes
 * @param in Info of the vertex to find.
 * @return Index of the vertex or -1 if it doesn't exists
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getInfo() == in)
            return i;
    return -1;
}

/**
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Complexity: O(v) where v is the number of vertexes in the graph.
 *  @param in Info of the vertex.
 *  @param type Type of hte new vertex.
 *  @return true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in, VertexType type) {
    if (findVertex(in) != nullptr)
        return false;
    vertexSet.push_back(new Vertex<T>(in,type));
    return true;
}

/**
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Complexity: O(v^2 + E^2) = O(v^2) where v is the number of vertexes and E is the number of edges.
 *  @param in Info of the vertex to remove.
 *  @return true if successful, and false if such vertex does not exist.
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

/**
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Complexity: O(v) where v is the number of vertexes in the graph.
 * @param dest info of the vertex that is the destination of the edge
 * @param sourc info of the vertex that is the source of the edge
 * @param w weight of the new edge.
 * @return true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

/**
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Complexity: O(E^2) where E is the number of edges.
 * @param dest info of the vertex that is the destination of the edge
 * @param sourc info of the vertex that is the source of the edge
 * @return true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> * srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

/**
 * Adds a bidirectional edge.
 * Complexity: O(V) where v is the number of vertexes.
 * @tparam T Type of class
 * @param sourc Source of the edge
 * @param dest Destination of the edge
 * @param w Weight of the edge
 * @return False if one of the vertexes (source or destination) doesn't exists. True otherwise
 */
template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

/****************** DFS ********************/

/**
 * Performs a depth-first search (dfs) traversal in a graph (this).
 * Complexity: O(V + E)
 * @return a vector with the contents of the vertices by dfs order.
 */
template <class T>
std::vector<T> Graph<T>::dfs() const {
    std::vector<T> res;
    for (auto v : vertexSet)
        v->setVisited(false);
    for (auto v : vertexSet)
        if (!v->isVisited())
            dfsVisit(v, res);
    return res;
}

/**
 * Performs a depth-first search (dfs) in a graph (this) from the source node.
 * Complexity: O(V + E)
 * @return a vector with the contents of the vertices by dfs order.
 */
template <class T>
std::vector<T> Graph<T>::dfs(const T & source) const {
    std::vector<int> res;
    // Get the source vertex
    auto s = findVertex(source);
    if (s == nullptr) {
        return res;
    }
    // Set that no vertex has been visited yet
    for (auto v : vertexSet) {
        v->setVisited(false);
    }
    // Perform the actual DFS using recursion
    dfsVisit(s, res);

    return res;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent, recursively.
 * Updates a parameter with the list of visited node contents.
 * Complexity: O(V + E)
 * @param v vertex we are now visiting
 * @param res vector with the vertex visited in DFS order
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> & res) const {
    v->setVisited(true);
    res.push_back(v->getInfo());
    for (auto & e : v->getAdj()) {
        auto w = e->getDest();
        if (!w->isVisited()) {
            dfsVisit(w, res);
        }
    }
}

/****************** BFS ********************/
/**
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Complexity: O(V + E)
 * @param source Info of the source vertex
 * @return a vector with the contents of the vertices by bfs order.
 */
template <class T>
std::vector<T> Graph<T>::bfs(const T & source) const {
    std::vector<int> res;
    // Get the source vertex
    auto s = findVertex(source);
    if (s == nullptr) {
        return res;
    }

    // Set that no vertex has been visited yet
    for (auto v : vertexSet) {
        v->setVisited(false);
    }

    // Perform the actual BFS using a queue
    std::queue<Vertex<T> *> q;
    q.push(s);
    s->setVisited(true);
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        res.push_back(v->getInfo());
        for (auto & e : v->getAdj()) {
            auto w = e->getDest();
            if ( ! w->isVisited()) {
                q.push(w);
                w->setVisited(true);
            }
        }
    }
    return res;
}

/****************** isDAG  ********************/
/**
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the stack of recursive calls (see theoretical classes).
 * Complexity: O(V + E)
 * @return true if the graph is acyclic, and false otherwise.
 */

template <class T>
bool Graph<T>::isDAG() const {
    for (auto v : vertexSet) {
        v->setVisited(false);
        v->setProcesssing(false);
    }
    for (auto v : vertexSet) {
        if (! v->isVisited()) {
            if ( ! dfsIsDAG(v) ) return false;
        }
    }
    return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent, recursively.
 * Complexity: O(V + E)
 * @param v vertex we are visiting
 * @return false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
    v->setVisited(true);
    v->setProcesssing(true);
    for (auto e : v->getAdj()) {
        auto w = e->getDest();
        if (w->isProcessing()) return false;
        if (! w->isVisited()) {
            if (! dfsIsDAG(w)) return false;
        }
    }
    v->setProcesssing(false);
    return true;
}

/****************** toposort ********************/
//=============================================================================
// Exercise 1: Topological Sorting
//=============================================================================
// TODO
/**
 * Performs a topological sorting of the vertices of a graph (this).
 * Complexity: O(V + E)
 * @return a vector with the contents of the vertices by topological order. If the graph has cycles, returns an empty vector.
 */

template<class T>
std::vector<T> Graph<T>::topsort() const {
    std::vector<int> res;

    for (auto v : vertexSet) {
        v->setIndegree(0);
    }
    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) {
            unsigned int indegree = e->getDest()->getIndegree();
            e->getDest()->setIndegree(indegree + 1);
        }
    }

    std::queue<Vertex<T> *> q;
    for (auto v : vertexSet) {
        if (v->getIndegree() == 0) {
            q.push(v);
        }
    }

    while( !q.empty() ) {
        Vertex<T> * v = q.front();
        q.pop();
        res.push_back(v->getInfo());
        for(auto e : v->getAdj()) {
            auto w = e->getDest();
            w->setIndegree(w->getIndegree() - 1);
            if(w->getIndegree() == 0) {
                q.push(w);
            }
        }
    }

    if ( res.size() != vertexSet.size() ) {
        //std::cout << "Impossible topological ordering!" << std::endl;
        res.clear();
        return res;
    }

    return res;
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