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
#include "DataSetSelection.h"

class WaterSupplyManagement {
public:
    //data readers
    void readReservoirs(DataSetSelection dataset);
    void readStations(DataSetSelection dataset);
    void readCities(DataSetSelection dataset);
    void readPipes(DataSetSelection dataset);

    //data inserters (to help filter the network)
    void insertReservoir(std::string code);
    void insertStation(std::string code);
    void insertCity(std::string code);
    void insertPipe(std::string code1, std::string code2);
    void insertAll();

    //Getters
    void getCity(const std::string& code, City *city) const;
    void getReservoir(std::string code, Reservoir *reservoir) const;
    void getStation(std::string code, Station *station) const;
    Graph<std::string> getNetwork() const;

    void selectDataSet(DataSetSelection dataset, std::string *filepath);
private:
    Graph<std::string> network;
    std::unordered_map<std::string, Reservoir> codeToReservoir;
    std::unordered_map<std::string, Station> codeToStation;
    std::unordered_map<std::string, City> codeToCity;
};


#endif //PROJECT1_WATERSUPPLYMANAGEMENT_H
