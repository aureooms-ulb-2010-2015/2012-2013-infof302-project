#ifndef SOLVER_TRANSLATOR_H
#define SOLVER_TRANSLATOR_H

#include <vector>
#include "SolverTypes.h"

class SolverTranslator{

public:
	template<typename Solver>
	static void toSolver(const std::vector<std::vector<int>>& clauses, Solver& solver){
		vec<Lit> lits;
		for(const std::vector<int>& clause : clauses){
			int var;
			lits.clear();
			for (int v : clause){
				var = abs(v)-1;
				while (var >= solver.nVars()) solver.newVar();
				lits.push( (v > 0) ? Lit(var) : ~Lit(var) );
			}
			solver.addClause(lits);
		}
	}

	
	template<typename Solver>
	static void fromSolver(const Solver& solver, Solution& solution){

		// TODO use solver.model[n° de variable]
	}
};


#endif //SOLVER_TRANSLATOR_H