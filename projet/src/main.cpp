#include "Exception.h"
#include "Terminal.h"
#include "Problem.h"
#include "ProblemParser.h"
#include "PrettyPrinter.h"
#include "ClausesGenerator.h"
#include "SolverTranslator.h"
#include "Solver.h"
#include "Solution.h"
#include "FileWriter.h"

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char* argv []){

	Terminal terminal;

	try{

		if(argc < 2) throw Exception("Missing input file in arguments");
		
		terminal.println("init ...", terminal.OKBLUE);
		ProblemParser parser;
		PrettyPrinter printer;
		ClausesGenerator generator;
		Solver solver;
		solver.verbosity = 0;

		std::string filename = argv[1];
		Problem problem = parser.parse(filename);
		std::vector<std::vector<int>> clauses = generator.run(problem);

		SolverTranslator::toSolver(clauses, solver);

		terminal.print("Solving ", terminal.OKBLUE);
		lbool ret = solver.solve();
		printer.print(ret);

		Solution solution;
		terminal.println("");
		SolverTranslator::fromSolver(solver, problem, solution);
		printer.print(solution, 1);
		terminal.println("");

		if(argc >= 3){
			terminal.println("Writing to file", terminal.OKBLUE);
			filename = argv[2];
			FileWriter writer;
			if(ret == l_True){
				problem.grid = solution.grid;
				writer.save(problem, filename);
			}
			else{
				writer.empty(filename);
			}
		}

		terminal.println("Done", terminal.OKBLUE);
	}
	catch(Exception& e){
		terminal.println("Error : " + e.what(), terminal.FAIL);
		return 1;
	}

	return 0;
}
