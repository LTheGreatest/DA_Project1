//
// Created by lucas on 10/03/2024.
//

#ifndef PROJECT1_CITY_H
#define PROJECT1_CITY_H

#include <string>

class City{
public:
    //Getters
    std::string getName() const {return name;}
    std::string getCode() const {return code;}
    int getId() const { return id;}
    int getPopulation() const {return population;}
    int getDemand() const {return demand;}

    //Setters
    void setName(const std::string &name) {this->name = name;}
    void setCode(const std::string &code) {this->code = code;}
    void setId(int id) {this->id = id;}
    void setPopulation(int population) {this->population = population;}
    void setDemand(int demand) {this->demand = demand;}

private:
    std::string name, code;
    int id, population, demand;

};
#endif //PROJECT1_CITY_H
