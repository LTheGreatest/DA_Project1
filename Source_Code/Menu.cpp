//
// Created by lucas on 14/03/2024.
//

#include "Menu.h"

using namespace std;

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

int Menu::mainMenu() {
    cout << "\nWELCOME TO THE WATER SUPPLY MANAGEMENT SYSTEM\n\n";

    while(true) {
        if (isSystemReset) {
            //when the system is reset or is started we need to select the data
            dataSelection();
        }

        cout << "Please insert the number corresponding to the option you want to select\n\n";

        cout << "1.Basic Metrics\n";
        cout << "2.Reliability and Sensitivity to Failures\n";
        cout << "3.Reset the system\n";
        cout << "4.Exit\n\n";

        int option;

        int s = inputCheck(option, 1, 4);
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

