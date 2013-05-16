#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include "Problem.h"
#include "Solution.h"

class PrettyPrinter{

public:

	virtual void print(const Problem&);
	virtual void print(const Solution&);
};

#endif // PRETTYPRINTER_H