//
// Created by lucas on 12/03/2024.
//


#include <gtest/gtest.h>
#include "WaterSupplyManagement.h"

WaterSupplyManagement testSystem;

void cleanSystem(){
    WaterSupplyManagement cleanSystem;
    testSystem = cleanSystem;
}

TEST(data_readers, readCitiesSmall){
    cleanSystem();

    testSystem.readCities(DataSetSelection::SMALL);

    City test1 {"Porto Moniz",1,"C_1",18,2517};
    City test2 {"Câmara de Lobos",7,"C_7",225,32162};

    City city1;
    City city2;
    testSystem.getCity("C_1", &city1);
    testSystem.getCity("C_7", &city2);

    EXPECT_EQ(city1, test1);
    EXPECT_EQ(city2, test2);
    EXPECT_EQ(testSystem.getCodeToCity().size(), 10);
}

TEST(data_readers, readCitiesBig){
    cleanSystem();

    testSystem.readCities(DataSetSelection::BIG);

    City test1 {"Alcacer do Sal",1,"C_1",52,7733};
    City test2 {"Viseu",22,"C_22",397,59469};

    City city1;
    City city2;
    testSystem.getCity("C_1", &city1);
    testSystem.getCity("C_22", &city2);

    EXPECT_EQ(city1, test1);
    EXPECT_EQ(city2, test2);
    EXPECT_EQ(testSystem.getCodeToCity().size(), 22);
}



TEST(data_readers, readReservoirSmall){
    cleanSystem();

    testSystem.readReservoirs(DataSetSelection::SMALL);
    Reservoir test1 {"Ribeira do Seixal","Porto Moniz",1,"R_1",525};
    Reservoir test2 {"Ribeiro Frio","Santana",4,"R_4",385};

    Reservoir reservoir1;
    Reservoir reservoir2;
    testSystem.getReservoir("R_1", &reservoir1);
    testSystem.getReservoir("R_4", &reservoir2);

    EXPECT_EQ(test1, reservoir1);
    EXPECT_EQ(test2, reservoir2);
    EXPECT_EQ(testSystem.getCodeToReservoir().size(), 4);
}

TEST(data_readers, readReservoirBig){
    cleanSystem();

    testSystem.readReservoirs(DataSetSelection::BIG);
    Reservoir test1 {"Ermida","Aveiro",1,"R_1",2750};
    Reservoir test2 {"Roxo","Beja",24,"R_24",64};

    Reservoir reservoir1;
    Reservoir reservoir2;
    testSystem.getReservoir("R_1", &reservoir1);
    testSystem.getReservoir("R_24", &reservoir2);

    EXPECT_EQ(test1, reservoir1);
    EXPECT_EQ(test2, reservoir2);
    EXPECT_EQ(testSystem.getCodeToReservoir().size(), 24);
}

TEST(data_readers, readStationsSmall){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    Station test1 {"PS_1", 1};
    Station test2 {"PS_12", 12};

    Station station1;
    Station station2;
    testSystem.getStation("PS_1", &station1);
    testSystem.getStation("PS_12", &station2);

    EXPECT_EQ(station1, test1);
    EXPECT_EQ(station2, test2);
    EXPECT_EQ(testSystem.getCodeToStation().size(), 12);
}

TEST(data_readers, readStationsBig){
    cleanSystem();

    testSystem.readStations(DataSetSelection::BIG);
    Station test1 {"PS_1", 1};
    Station test2 {"PS_81", 81};

    Station station1;
    Station station2;
    testSystem.getStation("PS_1", &station1);
    testSystem.getStation("PS_81", &station2);

    EXPECT_EQ(station1, test1);
    EXPECT_EQ(station2, test2);
    EXPECT_EQ(testSystem.getCodeToStation().size(), 81);
}

TEST(data_insert, insertAll){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    EXPECT_EQ(testSystem.getNetwork().getVertexSet().size(), 26);

}

TEST(data_readers, readPipes){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();

    testSystem.readPipes(DataSetSelection::SMALL);

    auto size = 0;

    for(Vertex<std::string> *vertex: testSystem.getNetwork().getVertexSet()){
        size += vertex->getAdj().size();
    }

    EXPECT_EQ(size, 51);

    Vertex<std::string> *v1 = testSystem.getNetwork().findVertex("R_1");
    Vertex<std::string> *v2 = testSystem.getNetwork().findVertex("PS_3");
    Vertex<std::string> *v3 = testSystem.getNetwork().findVertex("PS_4");

    for(auto e: v1->getAdj()){
        if(e->getDest()->getInfo() == "PS_1"){
            EXPECT_EQ(e->getWeight(), 100);
        }
    }

    for(auto e : v2->getAdj()){
        if(e->getDest()->getInfo() == "PS_4"){
            EXPECT_EQ(e->getWeight(), 250);
        }
    }

    for(auto e : v3->getAdj()){
        if(e->getDest()->getInfo() == "PS_3"){
            EXPECT_EQ(e->getWeight(), 250);
        }
    }
}

TEST(createSuper, createSuperSource){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();

    testSystem.readPipes(DataSetSelection::SMALL);

    testSystem.createSuperSource();
    Vertex<std::string> *superSource = testSystem.getNetwork().findVertex("super_source");
    EXPECT_EQ(superSource->getAdj().size(), 4);
}

TEST(createSuper, createSuperSink){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();

    testSystem.readPipes(DataSetSelection::SMALL);

    testSystem.createSuperSink();
    Vertex<std::string> *superSource = testSystem.getNetwork().findVertex("super_sink");
    EXPECT_EQ(superSource->getIncoming().size(), 10);
}

TEST(edmundsKarp, edmundsKarp){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    testSystem.edmondsKarp("super_source", "super_sink");

    for(std::pair<std::string, City> codeCity : testSystem.getCodeToCity()){
        Vertex<std::string> *cityVertex = testSystem.getNetwork().findVertex(codeCity.first);
        double waterAmount = 0;
        for(auto e: cityVertex->getIncoming()){
            waterAmount += e->getFlow();
        }
        std::cout << "city: " << codeCity.second.getName() << " water: " << waterAmount << '\n';
    }
}

TEST(edmundsKarp, edmundsKarpPipes){
    std::cout <<'\n' << "EdmundsKarpPipes" << '\n';
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    testSystem.edmondsKarp("super_source", "super_sink");

    for(auto v: testSystem.getNetwork().getVertexSet()){
        for(auto e: v->getAdj()){
            std::cout << "pipe -> "  << e->getOrig()->getInfo() << " : "  << e->getDest()->getInfo() << "   " << e->getFlow() << '\n';
        }
    }
}

TEST(basicMetrics, deficit){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    testSystem.edmondsKarp("super_source", "super_sink");

    double deficit1 = testSystem.flowDeficit("C_2");
    double deficit2 = testSystem.flowDeficit("C_6");

    double deficit3 = testSystem.flowDeficit("C_1");
    double deficit4 = testSystem.flowDeficit("C_3");
    double deficit5 = testSystem.flowDeficit("C_4");
    double deficit6 = testSystem.flowDeficit("C_5");
    double deficit7 = testSystem.flowDeficit("C_7");
    double deficit8 = testSystem.flowDeficit("C_8");
    double deficit9 = testSystem.flowDeficit("C_9");
    double deficit10 = testSystem.flowDeficit("C_10");

    EXPECT_EQ(deficit1, 0);
    EXPECT_EQ(deficit2, 76);
    EXPECT_EQ(deficit3, 0);
    EXPECT_EQ(deficit4, 0);
    EXPECT_EQ(deficit5, 0);
    EXPECT_EQ(deficit6, 0);
    EXPECT_EQ(deficit7, 0);
    EXPECT_EQ(deficit8, 0);
    EXPECT_EQ(deficit9, 0);
    EXPECT_EQ(deficit10, 0);

}

TEST(graphResiliency, affectedCitiesReservoir){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    double w = testSystem.getNetwork().findVertex("R_4")->getAdj().at(0)->getWeight();
    std::vector<std::pair<std::string, double>> previouslyAffected {std::make_pair("C_6",664)};
    std::vector<std::pair<std::string, double>> res = testSystem.affectedCitiesReservoir("R_4", previouslyAffected);

    EXPECT_EQ(res.size(), 3);
    EXPECT_EQ(testSystem.getNetwork().getVertexSet().size(), 28);
    EXPECT_EQ(w,testSystem.getNetwork().findVertex("R_4")->getAdj().at(0)->getWeight() );

    previouslyAffected = {std::make_pair("C_2",33)};
    res = testSystem.affectedCitiesReservoir("R_4", previouslyAffected);

    EXPECT_EQ(res.size(), 3);
}

TEST(resets, vertexreset){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    std::vector<std::string> destination {"PS_4", "PS_6", "PS_7", "PS_9", "PS_10"};
    std::vector<double> capacity {150, 100, 50, 100, 50};

    std::vector<std::pair<std::string, double>> previouslyAffected {std::make_pair("C_6",664)};
    testSystem.affectedCitiesReservoir("R_4",previouslyAffected);

    Vertex<std::string> *v = testSystem.getNetwork().findVertex("R_4");

    int i = 0;
    for(Edge<std::string> *e: v->getAdj()){
        EXPECT_EQ(e->getDest()->getInfo(), destination.at(i));
        EXPECT_EQ(e->getWeight(), capacity.at(i));
        i++;
    }
}

TEST(auxiliaryMetrics, avgPipes){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    testSystem.edmondsKarp("super_source", "super_sink");

    EXPECT_EQ(testSystem.avgDiffPipes() > 173, true);
}

TEST(auxiliaryMetrics, maxPipes){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    testSystem.edmondsKarp("super_source", "super_sink");

    EXPECT_EQ(testSystem.maxDiffPipes(), 750.0);
}

TEST(basicMetrics, rebalanceSmall){
    cleanSystem();

    testSystem.readStations(DataSetSelection::SMALL);
    testSystem.readReservoirs(DataSetSelection::SMALL);
    testSystem.readCities(DataSetSelection::SMALL);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::SMALL);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    testSystem.edmondsKarp("super_source", "super_sink");

    double avgInitial = testSystem.avgDiffPipes();
    double difInitial = testSystem.maxDiffPipes();
    testSystem.networkBalance();
    double avgFinal = testSystem.avgDiffPipes();
    double difFinal = testSystem.maxDiffPipes();

    std::cout << avgInitial << "   " << avgFinal << '\n';
    std::cout << difInitial << "   " << difFinal << '\n';

    EXPECT_EQ(avgFinal < avgInitial, true);
    EXPECT_EQ(difFinal <= difInitial, true);
}

TEST(basicMetrics, rebalanceBig){
    cleanSystem();

    testSystem.readStations(DataSetSelection::BIG);
    testSystem.readReservoirs(DataSetSelection::BIG);
    testSystem.readCities(DataSetSelection::BIG);
    testSystem.insertAll();
    testSystem.readPipes(DataSetSelection::BIG);
    testSystem.createSuperSource();
    testSystem.createSuperSink();

    testSystem.edmondsKarp("super_source", "super_sink");

    double avgInitial = testSystem.avgDiffPipes();
    double difInitial = testSystem.maxDiffPipes();
    testSystem.networkBalance();
    double avgFinal = testSystem.avgDiffPipes();
    double difFinal = testSystem.maxDiffPipes();

    std::cout << avgInitial << "   " << avgFinal << '\n';
    std::cout << difInitial << "   " << difFinal << '\n';

    EXPECT_EQ(avgFinal < avgInitial, true);
    EXPECT_EQ(difFinal <= difInitial, true);
}