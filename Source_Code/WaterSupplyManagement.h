//
// Created by lucas on 10/03/2024.
//

#ifndef PROJECT1_WATERSUPPLYMANAGEMENT_H
#define PROJECT1_WATERSUPPLYMANAGEMENT_H

#include "Graph.h"
#include "unordered_map"
#include "Reservoir.h"
#include "Station.h"
#include "City.h"


class WaterSupplyManagement {
public:
    Graph<std::string> getNetwork();
private:
    Graph<std::string> network;
    std::unordered_map<std::string, Reservoir> codeToReservoir;
    std::unordered_map<std::string, Station> codeToStation;
    std::unordered_map<std::string, City> codeToCity;
};


#endif //PROJECT1_WATERSUPPLYMANAGEMENT_H
