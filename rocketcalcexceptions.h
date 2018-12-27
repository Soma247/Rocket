#ifndef ROCKETCALCEXCEPTIONS_H
#define ROCKETCALCEXCEPTIONS_H
#include <exception>
#include <string>



class InvalidIALparameter:public std::exception{
    std::string message;
public:
    InvalidIALparameter(){}
    InvalidIALparameter(const std::string& fname);
    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT{
        return message.c_str();
    }
};
#endif // ROCKETCALCEXCEPTIONS_H
