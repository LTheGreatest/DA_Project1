//
// Created by lucas on 10/03/2024.
//

#ifndef PROJECT1_CITY_H
#define PROJECT1_CITY_H

#include <string>
/**
 * @file City.h
 * @brief Definition of class City.
 *
 * \class City
 * Where are stored and processed the information related to the Cities.
 */
class City{
public:

    City()= default;
    City(std::string name_, int id_, std::string code_, int demand_, int population_) :
        name(name_),id(id_), code(code_), demand(demand_), population(population_) {};

    //Getters ===================================================
    /**
     * Gets the city's name.
     * Complexity: O(1)
     * @return City's name
     */
    std::string getName() const {return name;}
    /**
     * Gets the city's code.
     * Complexity: O(1)
     * @return City's code
     */
    std::string getCode() const {return code;}
    /**
     * Gets city's id.
     * Complexity: O(1)
     * @return City's id
     */
    int getId() const { return id;}
    /**
     * Gets city's population.
     * Complexity: O(1)
     * @return City's population
     */
    int getPopulation() const {return population;}
    /**
     * Gets city's water demand.
     * Complexity: O(1)
     * @return City's water demand
     */
    int getDemand() const {return demand;}

    //Setters==============================================================
    /**
     * Sets city's name.
     * Complexity: O(1)
     * @param name New name for the city.
     */
    void setName(const std::string &name) {this->name = name;}
    /**
     * Sets the city's code.
     * Complexity: O(1)
     * @param code New code for the city.
     */
    void setCode(const std::string &code) {this->code = code;}
    /**
     * Sets the city's id.
     * Complexity: O(1)
     * @param id New id for the city.
     */
    void setId(int id) {this->id = id;}
    /**
     * Sets the city's population.
     * Complexity: O(1)
     * @param population New population for the city.
     */
    void setPopulation(int population) {this->population = population;}
    /**
     * Sets the city's water demand
     * Complexity: O(1)
     * @param demand New water demand for the city
     */
    void setDemand(int demand) {this->demand = demand;}

    //operator ===================================================================================
    bool operator==(const City &other) const{
        return (name == other.name) && (code == other.code) && (id == other.id) && (population == other.population) && (demand == other.demand);
    }

private:
    std::string name, code;
    int id, population, demand;

};
#endif //PROJECT1_CITY_H
