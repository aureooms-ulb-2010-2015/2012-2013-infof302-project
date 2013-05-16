#ifndef PROBLEMPARSER_H
#define PROBLEMPARSER_H

#include "Problem.h"
#include <string>
#include <fstream>
#include <vector>
#include "Exception.h"
#include <functional>



class ProblemParser{

public:
	class Exception : public ::Exception {
	public:
		Exception(const std::string& what): ::Exception(what) {}
	};

private:
	virtual void validate(const Problem& problem) const;
	virtual void validate(const std::vector<std::vector<int>>& values, size_t size, const std::string& type, std::function<bool(size_t,size_t)> comp) const;
	virtual void fill(std::ifstream& fs, size_t& val, char stop) const;
	virtual void fill(std::ifstream& fs, std::vector<std::vector<int>>& mat, size_t size) const;

public:
	virtual Problem parse(const std::string& filename) const;
};

#endif //PROBLEMPARSER_H