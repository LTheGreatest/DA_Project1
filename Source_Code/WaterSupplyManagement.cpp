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


