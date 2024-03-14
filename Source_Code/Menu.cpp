//
// Created by lucas on 14/03/2024.
//

#include "Menu.h"

using namespace std;

/** @file Menu.cpp
 *  @brief Implementation of Menu class
 */

/** Asks for an option (integer) and the user needs to write the option on the keyboard.
 * Complexity: O(1) (worst case is O(n) were n is the time the user writes wrong options)
 * @param option Were the option value is going to be stored
 * @param min   Minimum value accepted
 * @param max   Maximum value accepted
 * @return  If there was not any error 0. Else 1.
 */
int Menu::inputCheck(int &option, int min, int max) {
    while(cout << "Enter your Option:" ){
        if(not(cin >> option)) {
            if (cin.rdstate() & (std::ios::badbit | std::ios::eofbit)) {
                cout << "Aborted\n";
                return EXIT_FAILURE;
            }
            cout << "Please enter a valid Option (not an integer)\n";
            cin.clear();                            // clear the fail state
            // remove any lingering characters in the istream:
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if(option > max || option < min){
            cout << "Please enter a valid Option (integer out of range)\n";
            cin.clear();                            // clear the fail state
            // remove any lingering characters in the istream:
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else{
            //valid option inserted
            break;
        }
    }
    return EXIT_SUCCESS;
}

/** Main Menu of the system.
 *  Complexity: Varies on the functions called and how many times some of them are called.
 * @return If there was not any error 0. Else 1.
 */
int Menu::mainMenu() {
    cout << "\nWELCOME TO THE WATER SUPPLY MANAGEMENT SYSTEM\n\n";
    int s;
    while(true) {
        if (isSystemReset) {
            //when the system is reset or is started we need to select the data
            s = dataSelection();
            if (s != 0) {
                cout << "Error found\n";
                return EXIT_FAILURE;
            }
        }

        cout << "Please insert the number corresponding to the option you want to select\n\n";

        cout << "1.Basic Metrics\n";
        cout << "2.Reliability and Sensitivity to Failures\n";
        cout << "3.Reset the system\n";
        cout << "4.Exit\n\n";

        int option;

        s = inputCheck(option, 1, 4);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';

        switch (option) {
            case 1:
                s = basicMetrics();
                break;
            case 2:
                s = reliabilitySensivityFailure();
                break;
            case 3:
                isSystemReset = true;
                break;
            case 4:
                //exits the system
                return EXIT_SUCCESS;
            default:
                cout << "Error found\n";
                return EXIT_FAILURE;
        }

        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }

    }
}

int Menu::basicMetrics() {
    return 0;
}

int Menu::reliabilitySensivityFailure() {
    return 0;
}

int Menu::dataSelection() {
    return 0;
}
