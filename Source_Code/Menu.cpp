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
 *  Complexity: Varies on the functions called (depend on the users choice) and how many times some of them are called.
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

/**
 * The menu for the basic metrics.
 * Complexity: Varies on the functions (depend on the users choice) called and how many times some of them are called.
 * @return If there was not any error 0. Else 1.
 */
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

/**
 * Finds the cities that have water deficit to use in other menus.
 * Complexity: O(n^2)
 * @return Vector with a pair code/flow of each city
 */
vector<pair<string,double>> Menu::findAffectedCities(){
    vector<pair<string,double>> res;
    for(pair<string, City> codeCity : system.getCodeToCity()){
        double deficit = system.flowDeficit(codeCity.first);
        if( deficit > 0){
            res.emplace_back(codeCity.first, codeCity.second.getDemand() - deficit);
        }
    }
    return res;
}
/**
 * Finds the initial flow of each city before removing any reservoir, pipe or station to use in other menus.
 * Complexity: O(n)
 * @return Vector with a pair code/flow of each city
 */
std::vector<std::pair<std::string, double>> Menu::findInitialFlows() {
    vector<pair<string,double>> res;
    for(pair<string, City> codeCity : system.getCodeToCity()){
        double deficit = system.flowDeficit(codeCity.first);
        res.emplace_back(codeCity.first, codeCity.second.getDemand() - deficit);
    }
    return res;
}

/**
 * The menu for the reliability and Sensitivity to failure metrics.
 * Complexity: Varies on the functions (depend on the users choice) called and how many times some of them are called.
 * @return If there was not any error 0. Else 1.
 */
int Menu::reliabilitySensivityFailure() {
    while(true){
        cout << "\n RELIABILITY AND SENSITIVITY TO FAILURES MENU \n";

        cout << "1.Delivery capacity of the network if one specific water RESERVOIR is out of commission \n";
        cout << "2.Delivery capacity of the network if one specific water STATION is out of service (checks one by one) \n";
        cout << "3.PIPELINES, if ruptured, would make it impossible to deliver the desired amount of water to a given city \n";
        cout << "4.Delivery capacity of the network if one specific water STATION is out of service \n";
        cout << "5.Exit the menu\n";

        int s;
        int option;

        s = inputCheck(option, 1, 5);
        if (s != 0) {
            cout << "Error found\n";
            return EXIT_FAILURE;
        }
        cout << '\n';


        system.createSuperSource();
        system.createSuperSink();
        system.edmondsKarp("super_source", "super_sink");
        vector<pair<string,double>> affectedCities = findAffectedCities();
        vector<pair<string,double>> initialFlows = findInitialFlows();

        switch(option){
            case 1:
                affectCitiesReservoir(affectedCities, initialFlows);
                break;
            case 2:
                affectedCitiesStations(affectedCities);
                break;
            case 3:
                affectedCitiesPipes(affectedCities);
                break;
            case 4:
                affectedCitiesStation(affectedCities);
                break;
            case 5:
                return EXIT_SUCCESS;
        }

    }
}

//Basic metrics =========================================================================================================
/**
 * Submenu for finding the max flow.
 * Complexity: O(V + E) where E is the number of edges and v is the number of vertexes.
 * @return If there was not any error 0. Else 1.
 */
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

/**
 * Submenu for finding the water deficit.
 * Complexity: O(v^2) where v is the number of vertexes
 * @return If there was not any error 0. Else 1.
 */
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

/**
 * Submenu for balancing the network.
 * Complexity: O(VE^2 D) where v is the number of vertexes, E is the number of edges
 * and D is unknown (number of times the while loops runs, it depends on the balance of the original graph, worst case is exponential).
 * @return If there was not any error 0. Else 1.
 */
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

/**
 * Submenu to see the affected cities by removing a reservoir.
 * Complexity: O(V E^2) where V is the number of vertexes and E is the number of edges of the graph.
 * @param previouslyAffected Vector with pairs of city codes and their flow
 * @param initialFlows Vector with pairs of city codes and their initial flow before the removal of the reservoirs or pipes or stations.
 * @return If there was not any error 0. Else 1.
 */
int Menu::affectCitiesReservoir(vector<pair<string,double>> &previouslyAffected, const vector<pair<string,double>>& initialFlows ) {
    cout << "\nInsert the Reservoir code you want to delete from the system\n";
    string code;
    cin >> code;

    auto search = system.getCodeToReservoir().find(code);
    if(search == system.getCodeToReservoir().end()){
        cout << "This reservoir doesn't exists \n";
        return EXIT_FAILURE;
    }

    vector<pair<string,double>> affectedCities = system.affectedCitiesReservoir(code, previouslyAffected);

    cout << "Code, Name, PrevFlow, NewFlow\n";

    for(pair<string, double> codeFlow : affectedCities){
        auto codeReservoir = system.getCodeToCity().find(codeFlow.first);

        double initialFlow = 0;
        for(pair<string, double> codeInitialFlow : initialFlows){
            if(codeFlow.first == codeInitialFlow.first){
                initialFlow = codeInitialFlow.second;
            }
        }

        cout << codeFlow.first << ", " << codeReservoir->second.getName() << ", " <<initialFlow << ", " << codeFlow.second << '\n';

    }

    return EXIT_SUCCESS;

}

/**
 * Submenu to see the affected cities by removing a station (does this for all stations) and verifies if no cities were affected.
 * Complexity: O(V E^2) where V is the number of vertexes and E is the number of edges of the graph.
 * @param previouslyAffected Vector with pairs of city codes and their flow
 * @return If there was not any error 0. Else 1.
 */
int Menu::affectedCitiesStations(std::vector<std::pair<std::string,double>> &previouslyAffected) {
    vector<std::string> safeToDeleteStations;

    for(auto station: system.getCodeToStation()){
        vector<std::pair<std::string,double>> affectedCities=system.affectedCitiesStations(station.first,previouslyAffected);
        if(affectedCities.empty()){
            safeToDeleteStations.push_back(station.second.getCode());
        }
        else{
            cout<<"For the "<<station.second.getCode()<<" the affected cities are: \n";
            for(auto city : affectedCities){
                cout<<system.getCodeToCity()[city.first].getName()<<" with a deficit of "<<city.second<<"\n";
            }

        }
    }

    if(safeToDeleteStations.empty()){
       cout <<"There are no stations that don't cause deficit."<<"\n";
    }
    else{
        cout<<"The following stations do not cause deficit to appear in any city when removed:\n";
        for(std::string code: safeToDeleteStations){
            cout<<code<<"\n";
        }
    }
    return EXIT_SUCCESS;
}

/**
 * Submenu to see the affected cities by removing a station.
 * Complexity: O(V E^2) where V is the number of vertexes and E is the number of edges of the graph.
 * @param previouslyAffected Vector with pairs of city codes and their flow
 * @return If there was not any error 0. Else 1.
 */
int Menu::affectedCitiesStation(std::vector<std::pair<std::string, double>> &previouslyAffected) {
    string code;
    cout <<"\nPlease input the code of the station you want to remove\n";
    cin >> code;
    if(system.getCodeToStation().find(code) == system.getCodeToStation().end()){
        cout << "Station does not exist\n";
        return EXIT_FAILURE;
    }
    vector<std::pair<std::string,double>> affectedCities=system.affectedCitiesStations(code,previouslyAffected);
    if(affectedCities.empty()){
        cout<<"No cities were affected\n";
    }
    else{
        cout<<"For the "<<code<<" the affected cities are: \n";
        for(auto city : affectedCities){
            cout<<system.getCodeToCity()[city.first].getName()<<" with a deficit of "<<city.second<<"\n";
        }
    }

    return EXIT_SUCCESS;
}

/**
 * Submenu to see the affected cities by removing a pipe.
 * Complexity: O(V E^2) where V is the number of vertexes and E is the number of edges of the graph.
 * @param previouslyAffected Vector with pairs of city codes and their flow
 * @return If there was not any error 0. Else 1.
 */
int Menu::affectedCitiesPipes(std::vector<std::pair<std::string,double>> &previouslyAffected){
    vector<Edge<string>> edgesToRemove;
    bool keepAdding= true;
    cout <<"\nInput the code of the source of the edge you would like to remove\n";
    string code1;
    cin>>code1;
    if(system.getCodeToCity().find(code1) == system.getCodeToCity().end() && system.getCodeToReservoir().find(code1) == system.getCodeToReservoir().end() && system.getCodeToStation().find(code1) == system.getCodeToStation().end()){
        cout << "That source does not exist\n";
        return EXIT_FAILURE;
    }

    cout <<"\nInput the code of the destination of the edge you would like to remove\n";
    string code2;
    cin>>code2;

    if(system.getCodeToCity().find(code2) == system.getCodeToCity().end() && system.getCodeToReservoir().find(code2) == system.getCodeToReservoir().end() && system.getCodeToStation().find(code2) == system.getCodeToStation().end()){
        cout << "That source does not exist\n";
        return EXIT_FAILURE;
    }
    vector<pair<string, double>> affectedCities= system.crucialPipelines(code1,code2,previouslyAffected);
    cout << "The removed edges affect the following cities\n";

    if(affectedCities.empty()){
        cout << "No cities were affected\n";
    }
    else{
        for(const auto &city : affectedCities){
            cout << city.first <<" with a deficit of "<<city.second<<"\n";
        }
    }

    return EXIT_SUCCESS;
}

//Menu data selection and parsing =======================================================================================

/**
 * Submenu used to select the data to insert into the graph.
 * Complexity: O(n^2)
 * @return If there was not any error 0. Else 1.
 */
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

/** Submenu used to select reservoirs to insert into the graph.
 * Complexity: O(n^2)
 * @return If there was not any error 0. Else 1.
 */
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

/** Submenu used to select station to insert into the graph.
 * Complexity : O(n^2)
 * @return If there was not any error 0. Else 1.
 */
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

/** Submenu used to insert selected cities into the graph.
 * Complexity : O(n^2)
 * @return If there was not any error 0. Else 1.
 */
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

/** Submenu used to delete pipes from the graph.
 * Complexity: O(n^2)
 * @return If there was not any error 0. Else 1.
 */
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


