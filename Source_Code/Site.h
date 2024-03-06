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
        std::string getCode(){return code;}
        void setCode(std::string code_){code = code_;}
        bool operator==(const Site &other){return this->code == other.code;}
    private:
        std::string code;
};
#endif //PROJECT1_SITE_H
