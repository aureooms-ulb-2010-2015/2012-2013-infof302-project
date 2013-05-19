#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include "Problem.h"
#include "Solution.h"
#include "Terminal.h"

class PrettyPrinter{
private:
	Terminal* terminal;

public:
	PrettyPrinter(Terminal* terminal);
	virtual void print(const Problem&);
	virtual void print(const Solution&, size_t indent = 0);
	virtual void print(const bool);
	virtual void print(const std::vector<std::vector<int>>&);
};

#endif // PRETTYPRINTER_H