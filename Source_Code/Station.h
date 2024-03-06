#ifndef PROJECT1_STATION_H
#define PROJECT1_STATION_H
#include <string>
#include "Site.h"
class Station : public Site{
    public:
        Station(std::string code , int id_) : Site(code) , id(id_) {};
        int getStationId(){return id;}
        void setStationId(int id_){id = id_;}
    private:
        int id;
};


#endif //PROJECT1_STATION_H
