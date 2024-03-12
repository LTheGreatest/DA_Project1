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

TEST(data_readers, readCities){
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


