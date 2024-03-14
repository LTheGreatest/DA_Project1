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

    //Submenus for data selection
    int dataSelection();
    int selectReservoirs();
    int selectCities();
    int selectStations();
    int deletePipes();

    //auxiliary function to check the inputs
    int inputCheck(int &option, int min, int max);
private:
    WaterSupplyManagement system;
    bool isSystemReset = true;
};


#endif //PROJECT1_MENU_H
