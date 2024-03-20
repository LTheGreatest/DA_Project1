#ifndef PROJECT1_STATION_H
#define PROJECT1_STATION_H
#include <string>
/**
 * @file Station.h
 * @brief Definition of class Station.
 *
 * \class Station
 * Where are stored and processed the information related to the Stations.
 */
class Station{
    public:
        Station()=default;
        Station(std::string code_ , int id_) : code(code_) , id(id_) {};

        //Getters ============================================================================
        /**
         * Gets the station's id.
         * Complexity: O(1)
         * @return Station's id
         */
        int getStationId(){return id;}
        /**
         * Gets the station's code.
         * Complexity: O(1)
         * @return Station's code.
         */
        std::string getCode(){return code;}

        //Setters ============================================================================
        /**
         * Sets the station's code.
         * Complexity: O(1)
         * @param code_ New station code
         */
        void setCode(std::string code_){code = code_;}
        /**
         * Sets the station's id.
         * Complexity: O(1)
         * @param id_ New station id
         */
        void setStationId(int id_){id = id_;}

        //Operator ===========================================================================
        bool operator==(const Station &other) const{
            return (id == other.id) && (code == other.code);
        }
    private:
        int id;
        std::string code;
};


#endif //PROJECT1_STATION_H
