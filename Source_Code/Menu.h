//
// Created by lucas on 14/03/2024.
//

#ifndef PROJECT1_MENU_H
#define PROJECT1_MENU_H

#include "WaterSupplyManagement.h"
/**
 * @file Menu.h
 * @brief Definition of class Menu.
 *
 * \class Menu
 * Where are preformed functions related to the user interface(menus).
 * Stores a WaterSupplySystem object.
 */
class Menu {
public:

    //Constructor
    Menu()=default;

    //Menus
    int mainMenu();
    int basicMetrics();
    int reliabilitySensivityFailure();

    //Basic metrics
    int maxWater();
    int waterDeficit();
    int networkRebalance();

    //Reliability and Sensitivity to Failures

    int affectCitiesReservoir(std::vector<std::pair<std::string,double>> &previouslyAffected, const std::vector<std::pair<std::string, double>>& initialFlows );
    int affectedCitiesStations(std::vector<std::string> &previouslyAffected);
    int affectedCitiesPipes(std::vector<std::string> &previouslyAffected);


    //Submenus for data selection
    int dataSelection();
    int selectReservoirs();
    int selectCities();
    int selectStations();
    int deletePipes();

    //auxiliary
    std::vector<std::pair<std::string, double>> findAffectedCities();
    std::vector<std::pair<std::string, double>> findInitialFlows();

    //auxiliary function to check the inputs
    int inputCheck(int &option, int min, int max);
private:
    WaterSupplyManagement system;
    bool isSystemReset = true;
};


#endif //PROJECT1_MENU_H
