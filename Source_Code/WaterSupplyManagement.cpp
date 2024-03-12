//
// Created by lucas on 10/03/2024.
//

#include "WaterSupplyManagement.h"
#include <fstream>
#include <sstream>

using namespace std;

//Getters ============================================================================================

Graph<std::string> WaterSupplyManagement::getNetwork() const {
    return Graph<std::string>();
}

void WaterSupplyManagement::getCity(const std::string& code, City* city) const{
    auto search  = codeToCity.find(code);

    if(search != codeToCity.end()){
        *city = search->second;
    }
}

void WaterSupplyManagement::getReservoir(std::string code, Reservoir *reservoir) const {
    auto search = codeToReservoir.find(code);

    if(search != codeToReservoir.end()){
        *reservoir = search->second;
    }
}

void WaterSupplyManagement::getStation(std::string code, Station *station) const {
    auto search = codeToStation.find(code);

    if(search != codeToStation.end()){
        *station = search->second;
    }
}

void WaterSupplyManagement::selectDataSet(DataSetSelection dataset, std::string *filepath) {
    switch (dataset) {
        case DataSetSelection::SMALL:
            *filepath = "../SmallDataSet/Cities_Madeira.csv";
            break;
        case DataSetSelection::BIG:
            *filepath = "../LargeDataSet/Cities.csv";
            break;
    }
}
//data readers =========================================================================

void WaterSupplyManagement::readCities(DataSetSelection dataset) {
    string filepath;
    selectDataSet(dataset, &filepath);

    ifstream file(filepath);
    string line;

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


