#ifndef _CLAUSESGENERATOR_H
#define _CLAUSESGENERATOR_H

#include <vector>
#include <string>
#include "Problem.h"


class ClausesGenerator{
	
	private:
		void generateClausesRecursively(std::vector<std::vector<int>>& clauses);
		std::vector<std::vector<int>> generateClauses();

	public:
		std::vector<std::vector<int>> run(const Problem&);
};


#endif // _CLAUSESGENERATOR_H
