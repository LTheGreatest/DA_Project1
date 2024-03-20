#ifndef PROJECT1_RESERVOIR_H
#define PROJECT1_RESERVOIR_H
#include <string>
/**
 * @file Reservoir.h
 * @brief Definition of class Reservoir.
 *
 * \class Reservoir
 * Where are stored and processed the information related to the Reservoirs.
 */
class Reservoir{
    public:
        //Constructor
        Reservoir(std::string name_, std::string municipality_, int id_, std::string code_, double max_delivery_) : code(code_),name(name_) , municipality(municipality_) , id(id_) , max_delivery(max_delivery_) {};
        Reservoir()=default;

        //Getters ========================================================================================
        /**
         * Gets the reservoir's name.
         * Complexity: O(1)
         * @return Reservoir's name.
         */
        std::string getReservoirName(){ return name; }
        /**
        * Gets the reservoir's municipality.
        * Complexity: O(1)
        * @return Reservoir's municipality.
        */
        std::string getReservoirMunicipality(){ return municipality; }
        /**
        * Gets the reservoir's id.
        * Complexity: O(1)
        * @return Reservoir's id.
        */
        int getReservoirId(){ return id; }
        /**
        * Gets the reservoir's max water delivery capacity.
        * Complexity: O(1)
        * @return Reservoir's max water delivery capacity.
        */
        double getReservoirMaxDelivery(){ return max_delivery; }
        /**
        * Gets the reservoir's code.
        * Complexity: O(1)
        * @return Reservoir's code.
        */
        std::string  getCode(){ return code;}

        //Setters=================================================================
        /**
         * Sets the reservoir's name,
         * Complexity: O(1)
         * @param name_ New reservoir name.
         */
        void setReservoirName(std::string name_){name = name_;}
        /**
        * Sets the reservoir's municipality,
        * Complexity: O(1)
        * @param municipality__ New reservoir municipality.
        */
        void setReservoirMunicipality(std::string municipality_){municipality = municipality_;}
        /**
        * Sets the reservoir's id,
        * Complexity: O(1)
        * @param id_ New reservoir id.
        */
        void setReservoirId(int id_){id = id_;}
        /**
        * Sets the reservoir's max water delivery capacity.,
        * Complexity: O(1)
        * @param max_delivery_ New reservoir max water delivery capacity.
        */
        void setReservoirMaxDelivery(double max_delivery_){max_delivery = max_delivery_;}

        //Operator
        bool operator==(const Reservoir &other) const{
            return (name == other.name) && (municipality == other.municipality) && (code == other.code) && (id == other.id) && (max_delivery == other.max_delivery);
        }
    private:
        std::string name, municipality, code;
        int id;
        double max_delivery;
};
#endif //PROJECT1_RESERVOIR_H
