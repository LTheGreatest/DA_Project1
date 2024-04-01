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
            isSystemReset = false;
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
    while (true){
        cout << "\n BASIC METRICS MENU \n";

        cout << "1.Maximum amount of water that can reach each or a specific city\n";
        cout << "2.Water deficit \n";
        cout << "3.Network Balance \n";
        cout << "4.Store metrics to a file\n";
        cout << "5.Exit the menu\n";

        int s;
        int option;

        s = inputCheck(option, 1, 5);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';

        //prepares the system to execute the metrics
        system.createSuperSource();
        system.createSuperSink();
        system.edmondsKarp("super_source", "super_sink");

        switch(option){
            case 1:
                maxWater();
                break;
            case 2:
                waterDeficit();
                break;
            case 3:
                networkRebalance();
                break;
            case 4:
                cout << "\nThe metrics were stored in a file called metrics.csv\n";
                system.storeMetricsToFile();
                break;
            case 5:
                return EXIT_SUCCESS;
        }
    }
}


vector<string> Menu::findAffectedCities(){
    vector<string> res;
    for(pair<string, City> codeCity : system.getCodeToCity()){
        if(system.flowDeficit(codeCity.first) > 0){
            res.push_back(codeCity.first);
        }
    }
    return res;
}

int Menu::reliabilitySensivityFailure() {
    while(true){
        cout << "\n RELIABILITY AND SENSITIVITY TO FAILURES MENU \n";

        cout << "1.Delivery capacity of the network if one specific water RESERVOIR is out of commission \n";
        cout << "2.Delivery capacity of the network if one specific water STATION is out of service \n";
        cout << "3.PIPELINES, if ruptured, would make it impossible to deliver the desired amount of water to a given city \n";
        cout << "4.Exit the menu\n";

        int s;
        int option;

        s = inputCheck(option, 1, 4);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';


        system.createSuperSource();
        system.createSuperSink();
        system.edmondsKarp("super_source", "super_sink");
        vector<string> affectedCities = findAffectedCities();

        switch(option){
            case 1:
                affectCitiesReservoir(affectedCities);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                return EXIT_SUCCESS;
        }

    }
}

//Basic metrics =========================================================================================================

int Menu::maxWater() {
    cout << "Do you want to select a specific city?\n";

    cout << "1.Yes\n";
    cout << "2.No\n";

    int s;
    int option;

    s = inputCheck(option, 1, 2);
    if (s != 0) {
        cout << "Error found\n";
        return EXIT_FAILURE;
    }
    cout << '\n';

    double waterFlow;
    string code;
    Vertex<string> *v;
    switch(option){
        case 1:
            cout << "\nInsert the code of the city \n";
            cin >> code;
            v = system.getNetwork().findVertex(code);
            if(v == nullptr){
                cout << "\nCity doesn't exists\n";
                break;
            }
            waterFlow = 0;
            for(Edge<string> *e: v->getIncoming()){
                waterFlow += e->getFlow();
            }
            cout << "\nCode, Name, Water Amount \n";
            cout << code << ", " << system.getCodeToCity().find(code)->second.getName() << ", " << waterFlow << "\n";
            break;

        case 2:
            cout << "\nCode, Name, Water Amount \n";
            for(pair<string, City> codeCity : system.getCodeToCity()){
                v = system.getNetwork().findVertex(codeCity.first);
                if(v == nullptr) continue;
                waterFlow = 0;
                for(Edge<string> *e: v->getAdj()){
                    waterFlow += e->getFlow();
                }

                cout << codeCity.first << ", " << codeCity.second.getName() << ", " << waterFlow << "\n";
            }
            break;
    }

    return EXIT_SUCCESS;

}

int Menu::waterDeficit() {
    cout << "\nCode , Name,  Water deficit\n";
    double deficit = 0;
    for(pair<string, City> codeCity : system.getCodeToCity()){
        deficit = system.flowDeficit(codeCity.first);
        if(deficit > 0){
            cout << codeCity.first << ", " << codeCity.second.getName() << ", " << deficit << '\n';
        }
    }

    return EXIT_SUCCESS;
}

int Menu::networkRebalance() {

    double previousAVG = system.avgDiffPipes();
    double previousMaxDiff = system.maxDiffPipes();
    system.networkBalance();
    double newAVG = system.avgDiffPipes();
    double newMaxDiff = system.maxDiffPipes();


    cout << "\n\tAverage diff, " << "Maximum diff  \n";
    cout << "Before \t" << previousAVG << " \t" << previousMaxDiff << '\n';
    cout << "After  \t" << newAVG << " \t" << newMaxDiff << '\n';

    return 0;
}

//Reliability and Sensitivity to Failures ===============================================================================

int Menu::affectCitiesReservoir(vector<string> &previouslyAffected) {
    cout << "\nInsert the Reservoir code you want to delete from the system\n";
    string code;
    cin >> code;

    auto search = system.getCodeToReservoir().find(code);
    if(search == system.getCodeToReservoir().end()){
        cout << "This reservoir doesn't exists \n";
        return EXIT_FAILURE;
    }

    vector<string> affectedCities = system.affectedCitiesReservoir(code, previouslyAffected);

    cout << "Code, Name\n";

    for(string code : affectedCities){
        auto codeReservoir = system.getCodeToCity().find(code);

        cout << code << ", " << codeReservoir->second.getName() << '\n';

    }

    return EXIT_SUCCESS;

}

//Menu data selection and parsing =======================================================================================

int Menu::dataSelection() {
    cout << "\n SELECT THE DATA YOU WANT TO INSERT INTO THE SYSTEM \n";
    int s;

    cout << "1.Insert all available data\n";
    cout << "2.Insert personalized data\n";

    int option;

    s = inputCheck(option, 1, 2);
    if (s != 0) {
        cout << "Error found\n";
        return EXIT_FAILURE;
    }
    cout << '\n';

    system.resetSystem();
    system.readCities(DataSetSelection::BIG);
    system.readReservoirs(DataSetSelection::BIG);
    system.readStations(DataSetSelection::BIG);

    switch (option) {
        case 1:
            //inserts all the data available
            system.insertAll();
            system.readPipes(DataSetSelection::BIG);
            break;

        case 2:
            //inserts all the data manually (personalized by the user)
            selectCities();
            selectStations();
            selectReservoirs();
            system.readPipes(DataSetSelection::BIG);
            deletePipes();
            break;
    }

    return EXIT_SUCCESS;
}

int Menu::selectReservoirs() {
    while(true){
        string code;
        cout << "\n Write a reservoir code to insert into the system \n";
        cin >> code;

        auto search = system.getCodeToReservoir().find(code);
        if(search == system.getCodeToReservoir().end()){
            cout << "This code don't apply to any of the existent reservoirs! \n";
        }
        else{
            bool s = system.insertReservoir(code);
            if(!s){
                cout << "This reservoir is already in the system!\n";
            }
        }

        cout << "Do you want to continue inserting reservoirs ?\n";

        cout << "1.Yes\n";
        cout << "2.No\n";

        int s;
        int option;

        s = inputCheck(option, 1, 2);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';

        switch (option) {
            case 1:
                break;
            case 2:
                return EXIT_SUCCESS;
        }

    }
}

int Menu::selectStations() {
    while(true){
        string code;
        cout << "\n Write a station code to insert into the system \n";
        cin >> code;

        auto search = system.getCodeToStation().find(code);
        if(search == system.getCodeToStation().end()){
            cout << "This code don't apply to any of the existent station! \n";
        }
        else{
            bool s = system.insertStation(code);
            if(!s){
                cout << "This station is already in the system!\n";
            }
        }

        cout << "Do you want to continue inserting stations ?\n";

        cout << "1.Yes\n";
        cout << "2.No\n";

        int s;
        int option;

        s = inputCheck(option, 1, 2);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';

        switch (option) {
            case 1:
                break;
            case 2:
                return EXIT_SUCCESS;
        }

    }
}

int Menu::selectCities() {
    while(true){
        string code;
        cout << "\n Write a city code to insert into the system \n";
        cin >> code;

        auto search = system.getCodeToCity().find(code);
        if(search == system.getCodeToCity().end()){
            cout << "This code don't apply to any of the existent cities! \n";
        }
        else{
            bool s = system.insertCity(code);
            if(!s){
                cout << "This city is already in the system!\n";
            }
        }

        cout << "Do you want to continue inserting cities ?\n";

        cout << "1.Yes\n";
        cout << "2.No\n";

        int s;
        int option;

        s = inputCheck(option, 1, 2);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';

        switch (option) {
            case 1:
                break;
            case 2:
                return EXIT_SUCCESS;
        }

    }
}

int Menu::deletePipes() {
    while(true){
        cout << "\n Do you want to delete any pipe that exists in your current system configuration? \n";

        cout << "1.Yes\n";
        cout << "2.No\n";

        int s;
        int option;

        s = inputCheck(option, 1, 2);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';

        switch (option) {
            case 1:
                break;
            case 2:
                return EXIT_SUCCESS;
        }

        cout << "\n Insert the code of the source of the pipe \n";
        string source;
        cin >> source;
        cout << "\n Insert the code of the destination of the pipe \n";
        string destination;
        cin >> destination;

        if(!system.deletePipe(source, destination)){
            cout << "\n This pipe doesn't exists \n";
        }
    }
}


