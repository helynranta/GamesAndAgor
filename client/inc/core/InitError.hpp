#ifndef _INITERROR_HPP
#define _INITERROR_HPP

#include <iostream>
#include <string>
#include <exception>

class InitError: public std::exception {
private:
    std::string msg;
public:
    InitError();
    InitError(const std::string&);
    virtual ~InitError() throw();
    virtual const char* what() const throw();
};
#endif
