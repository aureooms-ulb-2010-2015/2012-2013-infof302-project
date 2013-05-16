#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception {
private:
	std::string _what;

public:
	Exception(const std::string& what):_what(what) {}
	virtual const std::string& what() const {return this->_what;}
};

#endif //EXCEPTION_H