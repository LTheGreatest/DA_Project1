//
// Created by lucas on 10/03/2024.
//

#include "WaterSupplyManagement.h"
#include <fstream>
#include <sstream>

using namespace std;

//Getters ============================================================================================
/**
 * Gets the water network/graph.
 * Complexity: O(1)
 * @return Water graph/network
 */
Graph<std::string> WaterSupplyManagement::getNetwork() const {
    return network;
}

/**
 * Gets a city with a specific code. If it doesn't exist return nullptr.
 * Complexity: O(1)
 * @param code Code of the city
 * @param city City that is returned
 */
void WaterSupplyManagement::getCity(const std::string& code, City* city) const{
    auto search  = codeToCity.find(code);

    if(search != codeToCity.end()){
        *city = search->second;
    }
}

/**
 * Gets a reservoir with a specific code. If it doesn't exist return nullptr.
 * Complexity: O(1)
 * @param code Code of the reservoir
 * @param reservoir Reservoir that is returned
 */
void WaterSupplyManagement::getReservoir(const std::string& code, Reservoir *reservoir) const {
    auto search = codeToReservoir.find(code);

    if(search != codeToReservoir.end()){
        *reservoir = search->second;
    }
}

/**
 * Gets a station with a specific code. If it doesn't exist return nullptr.
 * Complexity: O(1)
 * @param code Code of the station
 * @param station Station that is returned
 */
void WaterSupplyManagement::getStation(const std::string& code, Station *station) const {
    auto search = codeToStation.find(code);

    if(search != codeToStation.end()){
        *station = search->second;
    }
}

/**
 * Gets the hashmap with the code to reservoir information.
 * Complexity: O(1)
 * @return  code to reservoir hashmap
 */
unordered_map<std::string, Reservoir> WaterSupplyManagement::getCodeToReservoir() const {
    return codeToReservoir;
}

/**
 * Gets the hashmap with the code to station information.
 * Complexity: O(1)
 * @return code to station hashmap
 */
unordered_map<std::string, Station> WaterSupplyManagement::getCodeToStation() const {
    return codeToStation;
}

/**
 * Gets the hashmap with the code to city information.
 * Complexity: O(1)
 * @return code to city hashmap
 */
unordered_map<std::string, City> WaterSupplyManagement::getCodeToCity() const {
    return codeToCity;
}

/**
 * Used to select the path to the desired dataset.
 * Complexity: O(1)
 * @param dataset Which dataset we want (Big/Small)
 * @param type  What type of vertex we want to obtain the path to it's data file
 * @param filepath Path to the file
 */
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

/**
 * Inserts a reservoir in the network.
 * Complexity: O(n)
 * @param code Code of the reservoir
 * @return False if the reservoir already exists. True otherwise.
 */
bool WaterSupplyManagement::insertReservoir(const string& code) {
    Vertex<string> *reservoir = network.findVertex(code);

    if(reservoir != nullptr){
        //reservoir already exists
        return false;
    }

    network.addVertex(code, VertexType::RESERVOIR);
    return true;
}

/**
 * Inserts a station in the network.
 * Complexity: O(n)
 * @param code Code of the station
 * @return False if the reservoir already exists. True otherwise.
 */
bool WaterSupplyManagement::insertStation(const std::string& code) {
    Vertex<string> *station = network.findVertex(code);

    if(station != nullptr){
        //station already exists
        return  false;
    }

    network.addVertex(code, VertexType::STATIONS);
    return true;
}

/**
 * Inserts a city in the network.
 * Complexity: O(n)
 * @param code Code of the city
 * @return False if the city already exists. True otherwise.
 */
bool WaterSupplyManagement::insertCity(const std::string& code) {
    Vertex<string> *city = network.findVertex(code);

    if(city != nullptr){
        //city already exists
        return false;
    }

    network.addVertex(code, VertexType::CITIES);
    return true;
}

/**
 * Inserts all the reservoirs, cities and stations already stored in the hashmaps into the graph
 * Complexity: O(n^2)
 */
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
/**
 * Deletes a pipe in the network.
 * Complexity: O(n)
 * @param source Source vertex
 * @param dest  Destination vertex
 * @return  True if the removal was successful, false otherwise
 */
bool WaterSupplyManagement::deletePipe(const std::string &source, const std::string &dest) {
    return network.removeEdge(source, dest);
}

//Reset ===============================================================================
/**
 * Resets the system completely (nodes and edges).
 * Complexity: O(1)
 */
void WaterSupplyManagement::resetSystem() {
    Graph<string> newSystem;
    network = newSystem;
}

//super nodes ========================================================

/**
 * Creates a super source that connects all the sources(reservoirs) into a single source.
 * Complexity: O(n^2)
 */
void WaterSupplyManagement::createSuperSource() {
    if(network.addVertex("super_source", VertexType::SUPERSOURCE)) {

        for (pair<string, Reservoir> codeReservoir: codeToReservoir) {
            network.addEdge("super_source", codeReservoir.first, codeReservoir.second.getReservoirMaxDelivery());
        }
    }
}

/**
 * Creates a super sinks that connects all the sinks(cities) into a single sink.
 * Complexity: O(n^2)
 */
void WaterSupplyManagement::createSuperSink() {
    if(network.addVertex("super_sink", VertexType::SUPERSINK)) {

        for (pair<string, City> codeCity: codeToCity) {
            network.addEdge(codeCity.first, "super_sink", codeCity.second.getDemand());
        }
    }
}

/**
 * Removes the super source.
 * Complexity: O(n)
 */
void WaterSupplyManagement::removeSuperSource() {
    network.removeVertex("super_source");
}

/**
 * Removes the super sink.
 * Complexity: O(n)
 */
void WaterSupplyManagement::removeSuperSink() {
    network.removeVertex("super_sink");
}

//Edmunds Karp==============================================================================================================

/** Function to test the given vertex 'w' and visit it if conditions are met.
 *  Complexity: O(1)
 * @param q queue used in the BFS to find the augmenting path
 * @param e edge used to set the path
 * @param w vertex we are testing
 * @param residual  Residual capacity of the edge.
 */
void testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
    // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

/** Function to find an augmenting path using Breadth-First Search
 *  Complexity: O(E) where E is the number of edges of the Graph
 * @param g Graph where we are finding the augmenting path
 * @param s Source of the search.
 * @param t Target of the search.
 * @return  True if a path was found and false otherwise.
 */
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



/** Function to find the minimum residual capacity along the augmenting path
 *  Complexity: O(n) were n is the number of edges of the path.
 * @param s Source of the path
 * @param t Target of the path
 * @return  The minimal residual capacity along the path
 */
double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t) {
    double f = LONG_LONG_MAX;
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

/** Function to augment flow along the augmenting path with the given flow value
 *  Complexity: O(n) where n is the number of edges of the path
 * @param s source of the path
 * @param t target of the path
 * @param f Value of the flow we are augmenting
 */
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

/**
 *Executes the Edmonds Karp algorithm.
 * Complexity: O(V E^2)
 * @param source Starting point of the algorithm(super source)
 * @param target Finishing point of the algorithm(super sink)
 */
void WaterSupplyManagement::edmondsKarp(const string& source, const string& target) {
    // Find source and target vertices in the graph
    Vertex<string>* s = network.findVertex(source);
    Vertex<string>* t = network.findVertex(target);
    // Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");
    // Initialize flow on all edges to 0
    for (auto v : network.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    // While there is an augmenting path, augment the flow along the path
    while( findAugmentingPath(&network, s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}

//Basic Metrics =====================================================================================
/**
 * Calculates the flow deficit in a given city.
 * Complexity: O(n)
 * @param cityCode City we want to analise the flow deficit
 * @return The deficit value
 */
double WaterSupplyManagement::flowDeficit(const std::string& cityCode) {
    Vertex<string> *cityVertex = network.findVertex(cityCode);
    double deficit = codeToCity.find(cityCode)->second.getDemand();

    for(auto inc: cityVertex->getIncoming()){
        deficit -= inc->getFlow();
    }

    return deficit;
}


void WaterSupplyManagement::resetPath(){
    for(Vertex<string> *v : network.getVertexSet()){
        v->setPath(nullptr);
    }
}

void WaterSupplyManagement::resetVisited() {
    for(Vertex<string> *v : network.getVertexSet()){
        v->setVisited(false);
    }
}

void WaterSupplyManagement::networkBalance() {
    double Avg = avgDiffPipes();
    double initialAvg = Avg;
    double Diff = maxDiffPipes();

    resetPath();
    resetVisited();

    for(pair<string,Reservoir> codeR : codeToReservoir){
        Vertex<string> *v = network.findVertex(codeR.first);
        int maxTries = initialAvg;
        if(v->getAdj().size() > 1) {
            while (true) {

                //tries to find a path to subtract flow (smallest difference)
                if (!flowSub(v)) {
                    resetFlowChanges(v, 1);
                    break;
                }
                resetPath();
                resetVisited();

                //tries to find a path to add flow (biggest difference)
                if (!flowAdd(v)) {
                    resetFlowChanges(v, -1);
                    break;
                }
                resetPath();
                resetVisited();

                //calculates the new avg and verifies if it is better or worse than before
                double newAVG = avgDiffPipes();
                if(newAVG >= Avg){
                    break;
                }
                Avg = newAVG;
                maxTries--;
            }
        }
    }

    //calculates the difference of the pipes that go from the stations
    for(pair<string,Station> codeS : codeToStation){
        Vertex<string> *v = network.findVertex(codeS.first);
        int maxTries = initialAvg;
        if(v->getAdj().size() > 1) {
            while (true) {

                //tries to find a path to subtract flow (smallest difference)
                if (!flowSub(v)) {
                    resetFlowChanges(v, 1);
                    break;
                }
                resetPath();

                //tries to find a path to add flow (biggest difference)
                if (!flowAdd(v)) {
                    resetFlowChanges(v, -1);
                    break;
                }
                resetPath();

                //calculates the new avg and verifies if it is better or worse than before
                double newAVG = avgDiffPipes();
                if(newAVG >= Avg){
                    break;
                }
                Avg = newAVG;
                maxTries--;
            }
        }
    }
}


//Auxiliary functions to balance the network

bool WaterSupplyManagement::flowAdd(Vertex<std::string> *v) {
    Edge<string> *currentEdge;
    Vertex<string> *currentVertex = v;

    while(currentVertex->getType() != VertexType::CITIES){

        currentEdge = edgeWithTheMaxDiff(currentVertex->getAdj());

        if(currentEdge == nullptr){
            return false;
        }

        if(currentEdge->getWeight() - currentEdge->getFlow() == 0){
            return false;
        }

        currentEdge->setFlow(currentEdge->getFlow() + 1);
        currentVertex->setPath(currentEdge);
        currentVertex = currentEdge->getDest();
    }

    return true;
}

bool WaterSupplyManagement::flowSub(Vertex<std::string> *v) {
    Edge<string> *currentEdge;
    Vertex<string> *currentVertex = v;

    while(currentVertex->getType() != VertexType::CITIES){

        currentEdge = edgeWithTheMinDiff(currentVertex->getAdj());


        if(currentEdge == nullptr){
            cout << "This should not happen\n";
            return false;
        }

        if(currentEdge->getWeight() - currentEdge->getFlow() == currentEdge->getWeight()){
            return false;
        }

        currentEdge->setFlow(currentEdge->getFlow() -1);
        currentVertex->setPath(currentEdge);
        currentVertex = currentEdge->getDest();
    }

    return true;
}

void WaterSupplyManagement::resetFlowChanges(Vertex<std::string> *v, int flow) {
    Edge<string> *currentEdge;
    Vertex<string> *currentVertex = v;

    while(true){
        currentEdge = currentVertex->getPath();
        if (currentVertex->isVisited()){
            break;
        }

        if(currentEdge == nullptr){
            break;
        }

        currentEdge->setFlow(currentEdge->getFlow() + flow);
        currentVertex->setVisited(true);
        currentVertex = currentEdge->getDest();
    }

}

Edge<string>* WaterSupplyManagement::edgeWithTheMinDiff(std::vector<Edge<std::string> *> adj) {
    double diff = LONG_LONG_MAX;
    Edge<string>* res = nullptr;

    for(Edge<string> *e: adj){
        if((e->getWeight() - e->getFlow()) < diff){
            diff = e->getWeight() - e->getFlow();
            res = e;
        }
    }

    return res;
}

Edge<string>* WaterSupplyManagement::edgeWithTheMaxDiff(std::vector<Edge<std::string> *> adj) {
    double diff = 0.0;
    Edge<string>* res = nullptr;

    for(Edge<string> *e: adj){
        if((e->getWeight() - e->getFlow()) > diff){
            diff = e->getWeight() - e->getFlow();
            res = e;
        }
    }

    return res;
}


//Reliability and Sensitivity =========================================================================

/**
 * Gets the Cities that were affected (water supply not being met) by removing a given reservoir.
 * Complexity: O(V E^2) where V is the number of vertexes and E is the number of edges of the graph.
 * @param reservoirCode Code of the reservoir to be removed
 * @param previouslyAffected Vector with the code of the cities that were already with a water deficit before removing the reservoir
 * @return  Code of the cities that were affected by the removal of the reservoir.
 */
vector<string> WaterSupplyManagement::affectedCitiesReservoir(const string& reservoirCode, vector<string> &previouslyAffected) {
    vector<string> res;

    //Values that will be used to restore the graph in the end
    vector<double> weights;
    Vertex<string> *v = network.findVertex(reservoirCode);
    for(Edge<string> *e: v->getAdj()){
        weights.push_back(e->getWeight());
        e->setWeight(0);
    }

    //calculates the new flow (assumes that already exists a super_source and a super_sink)
    edmondsKarp("super_source", "super_sink");

    //verifies the cities with deficit and verifies if they were already with a deficit
    for(pair<string, City> codeCity : codeToCity){
        double deficit = flowDeficit(codeCity.first);
        if(deficit > 0){
            auto search = find(previouslyAffected.begin(), previouslyAffected.end(), codeCity.first);
            if(search == previouslyAffected.end()){
                res.push_back(codeCity.first);
            }
        }
    }

    //restores the Weights of the adj pipes
    int i = 0;
    for(Edge<string> *e: v->getAdj()){
        e->setWeight(weights.at(i));
        i++;
    }

    return res;
}

//auxiliary metrics ==================================================================================
/**
 * Calculates the average difference between the capacity and flow of each pipe.
 * Complexity: O(VE) where v is the number of vertexes (except the cities) and E is the number of edges
 * @return The average difference between the capacity and flow of each pipe
 */
double WaterSupplyManagement::avgDiffPipes() {
    int numPipes = 0;
    double sumDiff = 0.0;

    //calculates the difference of the pipes that go from the reservoirs
    for(pair<string,Reservoir> codeR : codeToReservoir){
        Vertex<string> *v = network.findVertex(codeR.first);
        for(Edge<string> *e : v->getAdj()){
            numPipes++;
            sumDiff += e->getWeight() - e->getFlow();
        }
    }

    //calculates the difference of the pipes that go from the stations
    for(pair<string,Station> codeS : codeToStation){
        Vertex<string> *v = network.findVertex(codeS.first);
        for(Edge<string> *e : v->getAdj()){
            numPipes++;
            sumDiff += e->getWeight() - e->getFlow();
        }
    }

    double avg = sumDiff/numPipes;

    return avg;

}

/**
 * Calculates the maximum difference between the capacity and flow of each pipe.
 * O(VE) where v is the number of vertexes (except the cities) and E is the number of edges
 * @return maximum difference between the capacity and flow of each pipe
 */
double WaterSupplyManagement::maxDiffPipes() {
    double maxDiff = 0.0;

    //calculates the difference of the pipes that go from the reservoirs
    for(pair<string,Reservoir> codeR : codeToReservoir){
        Vertex<string> *v = network.findVertex(codeR.first);
        for(Edge<string> *e : v->getAdj()){
             if (e->getWeight() - e->getFlow() > maxDiff){
                 maxDiff = e->getWeight() - e->getFlow();
             }
        }
    }

    //calculates the difference of the pipes that go from the stations
    for(pair<string,Station> codeS : codeToStation){
        Vertex<string> *v = network.findVertex(codeS.first);
        for(Edge<string> *e : v->getAdj()){
            if (e->getWeight() - e->getFlow() > maxDiff){
                maxDiff = e->getWeight() - e->getFlow();
            }
        }
    }

    return maxDiff;
}



