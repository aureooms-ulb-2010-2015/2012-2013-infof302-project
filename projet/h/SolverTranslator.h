#ifndef SOLVER_TRANSLATOR_H
#define SOLVER_TRANSLATOR_H

#include <vector>
#include "SolverTypes.h"

class SolverTranslator{

public:
	template<typename Solver>
	static void toSolver(const std::vector<std::vector<int>>& clauses, Solver& solver){
		Minisat::vec<Minisat::Lit> lits;
		for(const std::vector<int>& clause : clauses){
			int var;
			lits.clear();
			for (int v : clause){
				var = abs(v)-1;
				while (var >= solver.nVars()) solver.newVar();
				lits.push( (v > 0) ? Minisat::Lit(var) : ~Minisat::Lit(var) );
			}
			solver.addClause(lits);
		}
	}

	
	template<typename Solver>
	static void fromSolver(const Solver& solver, const Problem& problem, Solution& solution){
		solution.grid = problem.grid;
		size_t box = 0;
		for(size_t i = 0; i < problem.size; ++i){
			for(size_t j = 0; j < problem.size; ++j){
				if (solver.model[box] == Minisat::l_True) solution.grid[i][j] = 1;
				else if (solver.model[box] == Minisat::l_False) solution.grid[i][j] = -1;
				else solution.grid[i][j] = 0;
				++box;
			}
		}
	}
};


#endif //SOLVER_TRANSLATOR_H