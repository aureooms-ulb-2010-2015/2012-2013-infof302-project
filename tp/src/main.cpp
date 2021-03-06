
#include "Utils.h"
#include "ClausesGenerator.h"
#include "SolverTranslator.h"
#include "Solver.h"

int main(/*int argn,char ** args*/){

	std::vector<std::vector<char>>& table = Utils::MINES_SWEEPER_3;

	Utils::print(table);

	ClausesGenerator generator;

	std::vector<std::vector<int>> clauses = generator.run(table);
	
	Utils::toDIMACS(clauses);
	std::cout << Utils::toDIMACS(clauses);

	Solver solver;

	SolverTranslator::toSolver(clauses, solver);
	
	vec<Lit> dummy;
	lbool ret = solver.solve(dummy);
	printf(ret == l_True ? "SATISFIABLE\n" : ret == l_False ? "UNSATISFIABLE\n" : "INDETERMINATE\n");

	return 0;
}
