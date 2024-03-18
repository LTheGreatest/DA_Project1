//
// Created by lucas on 10/03/2024.
//

#include "WaterSupplyManagement.h"
#include <fstream>
#include <sstream>

using namespace std;

//Getters ============================================================================================

Graph<std::string> WaterSupplyManagement::getNetwork() const {
    return network;
}

void WaterSupplyManagement::getCity(const std::string& code, City* city) const{
    auto search  = codeToCity.find(code);

    if(search != codeToCity.end()){
        *city = search->second;
    }
}

void WaterSupplyManagement::getReservoir(const std::string& code, Reservoir *reservoir) const {
    auto search = codeToReservoir.find(code);

    if(search != codeToReservoir.end()){
        *reservoir = search->second;
    }
}

void WaterSupplyManagement::getStation(const std::string& code, Station *station) const {
    auto search = codeToStation.find(code);

    if(search != codeToStation.end()){
        *station = search->second;
    }
}

unordered_map<std::string, Reservoir> WaterSupplyManagement::getCodeToReservoir() const {
    return codeToReservoir;
}

unordered_map<std::string, Station> WaterSupplyManagement::getCodeToStation() const {
    return codeToStation;
}

unordered_map<std::string, City> WaterSupplyManagement::getCodeToCity() const {
    return codeToCity;
}

void WaterSupplyManagement::selectDataSet(DataSetSelection dataset, VertexType type, std::string *filepath) {
    switch (dataset) {
        case DataSetSelection::SMALL:
            switch (type) {
                case VertexType::RESERVOIR:
                    *filepath = "../SmallDataSet/Reservoirs_Madeira.csv";
                    break;
                case VertexType::STATIONS:
                    *filepath = "../SmallDataSet/Stations_Madeira.csv";
                    break;
                case VertexType::CITIES:
                    *filepath = "../SmallDataSet/Cities_Madeira.csv";
                    break;
                case VertexType::PIPE:
                    *filepath = "../SmallDataSet/Pipes_Madeira.csv";
                    break;
            }
            break;

        case DataSetSelection::BIG:
            switch (type) {
                case VertexType::RESERVOIR:
                    *filepath = "../LargeDataSet/Reservoir.csv";
                    break;
                case VertexType::STATIONS:
                    *filepath = "../LargeDataSet/Stations.csv";
                    break;
                case VertexType::CITIES:
                    *filepath = "../LargeDataSet/Cities.csv";
                    break;
                case VertexType::PIPE:
                    *filepath = "../LargeDataSet/Pipes.csv";
                    break;
            }
            break;
    }
}

//data readers =========================================================================
/** Reads data from the cities file and stores it in a hash map
 *  Complexity: O(n)
 */
void WaterSupplyManagement::readCities(DataSetSelection dataset) {
    string filepath;
    selectDataSet(dataset, VertexType::CITIES, &filepath);

    ifstream file(filepath);
    if(!file.is_open()){
        cerr << "Error: Unable to open the file." << '\n';
    }

    string line;

    getline(file,line); //header line

    std::string name, code;
    int id, population, demand;
    while(getline(file,line)){

        //get name
        size_t it = line.find_first_of(',');
        name = line.substr(0,it);
        line = line.substr(it + 1);

        //get id
        it = line.find_first_of(',');
        id = stoi(line.substr(0,it));
        line = line.substr(it + 1);

        //get code
        it = line.find_first_of(',');
        code = line.substr(0,it);
        line = line.substr(it + 1);

        //get demand
        it = line.find_first_of(',');
        demand = stoi(line.substr(0,it));
        line = line.substr(it + 1);

        //get population
        population = stoi(line);

        //construct the city and puts it in the hash table
        City city {name, id, code, demand, population};
        codeToCity.emplace(code, city);
    }
}

/** Reads data from the reservoirs file and stores it in a hash map
 *  Complexity: O(n)
 */
void WaterSupplyManagement::readReservoirs(DataSetSelection dataset) {
    string filepath;
    selectDataSet(dataset,VertexType::RESERVOIR, &filepath);

    ifstream file(filepath);
    if(!file.is_open()){
        cerr << "Error: Unable to open the file." << '\n';
    }

    string line;

    getline(file,line); //header line

    std::string name, municipality, code;
    int id;
    double max_delivery;

    while(getline(file, line)){

        //get name
        size_t it = line.find_first_of(',');
        name = line.substr(0,it);
        line = line.substr(it + 1);

        //get municipality
        it = line.find_first_of(',');
        municipality = line.substr(0,it);
        line = line.substr(it + 1);

        //get id
        it = line.find_first_of(',');
        id = stoi(line.substr(0,it));
        line = line.substr(it + 1);

        //get code
        it = line.find_first_of(',');
        code = line.substr(0,it);
        line = line.substr(it + 1);

        //get max delivery
        max_delivery = stod(line);

        //construct the reservoir and emplace it in the hash map
        Reservoir reservoir {name, municipality, id, code ,max_delivery};
        codeToReservoir.emplace(code, reservoir);
    }

}

/** Reads data from the stations file and stores it in a hash map
 *  Complexity: O(n)
 */
void WaterSupplyManagement::readStations(DataSetSelection dataset) {
    string filepath;
    selectDataSet(dataset,VertexType::STATIONS, &filepath);

    ifstream file(filepath);
    if(!file.is_open()){
        cerr << "Error: Unable to open the file." << '\n';
    }

    string line;

    getline(file,line); //header line

    int id;
    string code;
    while(getline(file, line)){

        //get id
        size_t it = line.find_first_of(',');
        id = stoi(line.substr(0,it));
        line = line.substr(it + 1);

        //get code
        code = line;

        //construct the station and emplace it in the hash map
        Station station {code ,id};
        codeToStation.emplace(code, station);
    }
}

/** Reads data from the pipes file and creates edges in the graph with the data read
 *  Complexity: O(n)
 */
void WaterSupplyManagement::readPipes(DataSetSelection dataset) {
    string filepath;
    selectDataSet(dataset,VertexType::PIPE, &filepath);

    ifstream file(filepath);
    if(!file.is_open()){
        cerr << "Error: Unable to open the file." << '\n';
    }

    string line;

    getline(file,line); //header line

    string destCode, origCode;
    double capacity;
    int direction;

    while(getline(file, line)){

        //get origCode
        size_t it = line.find_first_of(',');
        origCode = line.substr(0,it);
        line = line.substr(it + 1);

        //get destCode
        it = line.find_first_of(',');
        destCode = line.substr(0,it);
        line = line.substr(it + 1);

        //get capacity
        it = line.find_first_of(',');
        capacity = stod(line.substr(0,it));
        line = line.substr(it + 1);

        //get direction
        direction = stoi(line);

        //adds the pipe to the network (and possibly the reverse)
        network.addEdge(origCode,destCode, capacity);
        if(direction == 0){
            network.addEdge(destCode, origCode, capacity);
        }
    }
}



//Data insertion ================================================================================================

bool WaterSupplyManagement::insertReservoir(string code) {
    Vertex<string> *reservoir = network.findVertex(code);

    if(reservoir != nullptr){
        //reservoir already exists
        return false;
    }

    network.addVertex(code, VertexType::RESERVOIR);
    return true;
}

bool WaterSupplyManagement::insertStation(std::string code) {
    Vertex<string> *station = network.findVertex(code);

    if(station != nullptr){
        //station already exists
        return  false;
    }

    network.addVertex(code, VertexType::STATIONS);
    return true;
}

bool WaterSupplyManagement::insertCity(std::string code) {
    Vertex<string> *city = network.findVertex(code);

    if(city != nullptr){
        //city already exists
        return false;
    }

    network.addVertex(code, VertexType::CITIES);
    return true;
}

void WaterSupplyManagement::insertAll() {

    //insert cities
    for(pair<string, City> codeCity : codeToCity){
        insertCity(codeCity.first);
    }

    //insert stations
    for(pair<string, Station> codeStation : codeToStation){
        insertStation(codeStation.first);
    }

    //insert reservoir
    for(pair<string, Reservoir> codeReservoir : codeToReservoir){
        insertReservoir(codeReservoir.first);
    }
}

//Deletes =============================================================================
bool WaterSupplyManagement::deletePipe(const std::string &source, const std::string &dest) {
    return network.removeEdge(source, dest);
}

//Reset ===============================================================================
void WaterSupplyManagement::resetSystem() {
    Graph<string> newSystem;
    network = newSystem;
}

//super nodes

void WaterSupplyManagement::createSuperSource() {
    network.addVertex("super_source", VertexType::SUPERSOURCE);

    for(pair<string, Reservoir> codeReservoir: codeToReservoir){
        network.addEdge("super_source", codeReservoir.first, codeReservoir.second.getReservoirMaxDelivery());
    }
}

void WaterSupplyManagement::createSuperSink() {
    network.addVertex("super_sink", VertexType::SUPERSINK);

    for(pair<string, City> codeCity: codeToCity){
        network.addEdge(codeCity.first, "super_sink", codeCity.second.getDemand());
    }
}

void WaterSupplyManagement::removeSuperSource() {
    network.removeVertex("super_source");
}

void WaterSupplyManagement::removeSuperSink() {
    network.removeVertex("super_sink");
}

//Edmunds Karp==============================================================================================================

// Function to test the given vertex 'w' and visit it if conditions are met
void testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual) {
// Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
// Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

// Function to find an augmenting path using Breadth-First Search
bool findAugmentingPath(Graph<string> *g, Vertex<string> *s, Vertex<string> *t) {
// Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
    }
// Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<string> *> q;
    q.push(s);
// BFS to find an augmenting path
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
// Process outgoing edges
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
// Process incoming edges
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
// Return true if a path to the target is found, false otherwise
    return t->isVisited();
}



// Function to find the minimum residual capacity along the augmenting path
double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t) {
    double f = LONG_LONG_MIN;
// Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
// Return the minimum residual capacity
    return f;
}

// Function to augment flow along the augmenting path with the given flow value
void augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f) {
// Traverse the augmenting path and update the flow values accordingly
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}


void WaterSupplyManagement::edmondsKarp(Graph<string> *g, string source, string target) {
// Find source and target vertices in the graph
    Vertex<string>* s = g->findVertex(source);
    Vertex<string>* t = g->findVertex(target);
// Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");
// Initialize flow on all edges to 0
    for (auto v : g->getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
// While there is an augmenting path, augment the flow along the path
    while( findAugmentingPath(g, s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}



