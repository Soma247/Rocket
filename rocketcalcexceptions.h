#ifndef ROCKETCALCEXCEPTIONS_H
#define ROCKETCALCEXCEPTIONS_H
#include <exception>
#include <string>
class IDexception:public std::exception{
  std::string msg;
public:
  IDexception(){}
  IDexception(const std::string& str):msg{str}{}
  virtual const char* what() const _GLIBCXX_USE_NOEXCEPT{
      return msg.c_str();
  }
};


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
