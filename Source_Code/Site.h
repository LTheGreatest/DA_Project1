//
// Created by Utilizador on 3/5/2024.
//
#include <string>
#ifndef PROJECT1_SITE_H
#define PROJECT1_SITE_H
class Site {
    public:
        Site(std::string code_) : code(code_) {};
        virtual ~Site();
        std::string getReservoirCode(){return code;}
        void setReservoirCode(std::string code_){code = code_;}
    private:
        std::string code;
};
#endif //PROJECT1_SITE_H
