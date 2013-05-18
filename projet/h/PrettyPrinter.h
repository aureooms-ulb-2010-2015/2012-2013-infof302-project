#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include "Problem.h"
#include "Solution.h"
#include "SolverTypes.h"

class PrettyPrinter{

public:

	virtual void print(const Problem&);
	virtual void print(const Solution&);
	virtual void print(const lbool&);
	virtual void print(const std::vector<std::vector<int>>&);
};

#endif // PRETTYPRINTER_H