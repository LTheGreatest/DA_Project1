#ifndef PROJECT1_STATION_H
#define PROJECT1_STATION_H
#include <string>

class Station{
    public:
        Station()=default;
        Station(std::string code_ , int id_) : code(code_) , id(id_) {};
        int getStationId(){return id;}
        void setStationId(int id_){id = id_;}

        //Operator
        bool operator==(const Station &other) const{
            return (id == other.id) && (code == other.code);
        }
    private:
        int id;
        std::string code;
};


#endif //PROJECT1_STATION_H
