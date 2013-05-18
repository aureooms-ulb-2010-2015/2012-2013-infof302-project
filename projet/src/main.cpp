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

		
		std::cout << "Init parser" << std::endl;
		ProblemParser parser;
		std::cout << "Init printer" << std::endl;
		PrettyPrinter printer;
		std::cout << "Init generator" << std::endl;
		ClausesGenerator generator;
		std::cout << "Init solver" << std::endl;
		Solver solver;

		std::string filename = argv[1];
		std::cout << "Parsing file" << std::endl;
		Problem problem = parser.parse(filename);
		printer.print(problem);
		std::cout << "Generating clauses" << std::endl;
		std::vector<std::vector<int>> clauses = generator.run(problem);

		std::cout << "Passing clauses to solver" << std::endl;
		SolverTranslator::toSolver(clauses, solver);

		std::cout << "Solving" << std::endl;
		lbool ret = solver.solve();
		printer.print(ret);

		Solution solution;
		std::cout << "Parsing solution" << std::endl;
		SolverTranslator::fromSolver(solver, problem, solution);
		printer.print(solution);
	}
	catch(Exception& e){
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}
