#ifndef SOLVER_TRANSLATOR_H
#define SOLVER_TRANSLATOR_H

#include "SolverTypes.h"

class SolverTranslator{

public:
	template<typename Solver>
	static void toSolver(const std::vector<std::vector<int>>& clauses, Solver& S){
		vec<Lit> lits;
		for(const std::vector<int>& clause : clauses){
			int var;
			lits.clear();
			for (int v : clause){
				var = abs(v)-1;
				while (var >= S.nVars()) S.newVar();
				lits.push( (v > 0) ? Lit(var) : ~Lit(var) );
			}
			S.addClause(lits);
		}
	}


};


#endif //SOLVER_TRANSLATOR_H