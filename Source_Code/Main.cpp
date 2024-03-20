//
// Created by lucas on 05/03/2024.
//
#include <iostream>
#include "graph.h"
#include "Menu.h"
/**
 * @file Main.cpp
 * @brief This file contains the main function of the project.
 *
  * \mainpage Description
 * This project consists in a set of classes and functions that allow the user to interact with a water supply management system.
 * With this simple system, you can access some basic metrics about the system and other information related to the reliability of the system.
 *
 * \subsection Instructions How to use
 * You will be asked to choose an option. All the options will appear on the screen with a number near them.
 * Then, just type the number corresponding to the option you want to execute.
 */

int main(){
    Menu menu;
    return menu.mainMenu();
}
