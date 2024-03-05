#ifndef PROJECT1_RESERVOIR_H
#define PROJECT1_RESERVOIR_H
#include <string>
#include "Site.h"
class Reservoir : public Site{
    public:
        //Constructor
        Reservoir(std::string name_, std::string municipality_, int id_, std::string code_, double max_delivery_) : Site(code_),name(name_) , municipality(municipality_) , id(id_) , max_delivery(max_delivery_) {};
        //Getters
        std::string getReservoirName(){return name;}
        std::string getReservoirMunicipality(){return municipality;}
        int getReservoirId(){return id;}
        double getReservoirMaxDelivery(){return max_delivery;}
        //Setters
        void setReservoirName(std::string name_){name = name_;}
        void setReservoirMunicipality(std::string municipality_){municipality = municipality_;}
        void setReservoirId(int id_){id = id_;}
        void setReservoirMaxDelivery(double max_delivery_){max_delivery = max_delivery_;}
        virtual void f(){};
    private:
        std::string name, municipality;
        int id;
        double max_delivery;
};
#endif //PROJECT1_RESERVOIR_H
