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
    /**
 * @file WaterSupplyManagement.h
 * @brief Definition of class WaterSupplyManagement.
 *
 * \class WaterSupplyManagement
 * Where are preformed most important functions related to the system (metrics and reliability)
 * Stores a network graph and some unordered maps to help find the information related to each vertex.
 */
public:
    WaterSupplyManagement()= default;
    //data readers
    void readReservoirs(DataSetSelection dataset);
    void readStations(DataSetSelection dataset);
    void readCities(DataSetSelection dataset);
    void readPipes(DataSetSelection dataset);

    //data inserts and deletes (to help filter the network)
    bool insertReservoir(const std::string& code);
    bool insertStation(const std::string& code);
    bool insertCity(const std::string& code);
    void insertAll();
    bool deletePipe(const std::string &source, const std::string &dest);

    //System reset
    void resetSystem();
    void restoreVertex(const std::string& code, VertexType type,  const std::vector<Edge<std::string>>& incomingEdges, const std::vector<Edge<std::string>>& outGoingEdges);

    //Getters
    void getCity(const std::string& code, City *city) const;
    void getReservoir(const std::string& code, Reservoir *reservoir) const;
    void getStation(const std::string& code, Station *station) const;
    std::unordered_map<std::string, Reservoir> getCodeToReservoir() const;
    std::unordered_map<std::string, Station> getCodeToStation() const;
    std::unordered_map<std::string, City> getCodeToCity() const;
    Graph<std::string> getNetwork() const;

    //super nodes
    void createSuperSource();
    void createSuperSink();
    void removeSuperSource();
    void removeSuperSink();

    //EdmundsKarp
    void edmondsKarp(const std::string& source, const std::string& target);

    //Basic metrics
    double flowDeficit(const std::string& cityCode );

    //Reliability and Sensitivity
    std::vector<std::string> affectedCitiesReservoir(const std::string& reservoirCode, std::vector<std::string> &previouslyAffected);


    static void selectDataSet(DataSetSelection dataset, VertexType type, std::string *filepath);
private:
    Graph<std::string> network;
    std::unordered_map<std::string, Reservoir> codeToReservoir;
    std::unordered_map<std::string, Station> codeToStation;
    std::unordered_map<std::string, City> codeToCity;
};


#endif //PROJECT1_WATERSUPPLYMANAGEMENT_H
