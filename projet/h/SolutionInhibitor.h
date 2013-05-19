#ifndef SOLUTIONINHIBITOR_H
#define SOLUTIONINHIBITOR_H

#include "FNC.h"
#include "Solution.h"

class SolutionInhibitor{

	public:
		virtual void inhibit(const Solution& solution, FNC& constraints, const ClausesGenerator& generator){
			Clause clause;
			for(size_t i = 0; i < solution.grid.size(); ++i){
				for(size_t j = 0; j < solution.grid.at(i).size(); ++j){
					if(solution.grid.at(i).at(j) == 1) clause.push_back(-generator.BOX(i,j));
					else if (solution.grid.at(i).at(j) == -1) clause.push_back(generator.BOX(i,j));
				}
			}

			constraints.push_back(clause);
		}
};



#endif //SOLUTIONINHIBITOR_H