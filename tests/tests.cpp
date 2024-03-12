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


