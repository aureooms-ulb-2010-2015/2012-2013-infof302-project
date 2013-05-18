#include "Exception.h"
#include "Problem.h"
#include "ProblemParser.h"
#include "PrettyPrinter.h"
#include "ClausesGenerator.h"
#include "SolverTranslator.h"
#include "Solver.h"
#include "Solution.h"

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char* argv []){

	try{

		if(argc < 2){
			std::cout << "Error : Missing grid file in arguments" << std::endl;
			return 1;
		}


		ProblemParser parser;
		PrettyPrinter printer;
		ClausesGenerator generator;
		Solver solver;

		std::string filename = argv[1];
		Problem problem = parser.parse(filename);
		printer.print(problem);
		std::vector<std::vector<int>> clauses = generator.run(problem);

		SolverTranslator::toSolver(clauses, solver);

		lbool ret = solver.solve();
		printer.print(ret);

		Solution solution;
		SolverTranslator::fromSolver(solver, solution);
		printer.print(solution);
	}
	catch(Exception& e){
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}