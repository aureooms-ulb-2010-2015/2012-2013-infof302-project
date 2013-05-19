#include "Exception.h"
#include "InputParser.h"
#include "Chrono.h"
#include "Terminal.h"
#include "Problem.h"
#include "ProblemParser.h"
#include "PrettyPrinter.h"
#include "FNC.h"
#include "ClausesGenerator.h"
#include "SolverTranslator.h"
#include "SolutionInhibitor.h"
#include "Solver.h"
#include "Solution.h"
#include "FileWriter.h"

#include <vector>
#include <string>
#include <iostream>


void HELP();
void CONFIGURE();
void PREPARE();
void RUN();
void FINISH();

Terminal TERMINAL;
Chrono CHRONO;
std::unordered_set<std::string> F = {"-v", "-t", "-h", "-u", "--help", "--nocolor"};

int main(int argc, char* argv []){
	PARSEINPUT(argc, argv, F);
	CONFIGURE();
	PREPARE();

	if(FLAG(-h) || FLAG(--help)){
        HELP();
        return 0;
    }

	try{
		if(ARGV.empty()) throw Exception("Missing input file in arguments");
		RUN();
	}
	catch(Exception& e){
		TERMINAL.println("Error : " + e.what(), TERMINAL.FAIL);
		return 1;
	}

	FINISH();
	return 0;
}


void HELP(){
    std::cout << "Liste des options :" << std::endl;
    std::cout << "  -u : test unicity" << std::endl;
    std::cout << "  -v : verbose" << std::endl;
    std::cout << "  -t : show time" << std::endl;
    std::cout << "  -h --help : show help" << std::endl;
    std::cout << "  --nocolor : don't print in colors" << std::endl;
}

void CONFIGURE(){
    if(FLAG(--nocolor)) TERMINAL.colors = false;
}
void PREPARE(){
    std::cout << std::boolalpha;
    srand(time(0));
    if(FLAG(-t)) CHRONO.start();
}

void RUN(){
	TERMINAL.println("init ...");
	ProblemParser parser;
	PrettyPrinter printer(&TERMINAL);
	ClausesGenerator generator;
	std::vector<Solution> solutions;
	Minisat::Solver solver;
	solver.verbosity = 0;

	std::string filename = ARGV[0];
	Problem problem = parser.parse(filename);
	FNC clauses = generator.run(problem);

	SolverTranslator::toSolver(clauses, solver);

	TERMINAL.print("Solving ");
	solver.solve();
	printer.print(solver.okay());


	if(solver.okay()){

		Solution solution;
		TERMINAL.println("");
		SolverTranslator::fromSolver(solver, problem, solution);
		printer.print(solution, 1);
		TERMINAL.println("");

		solutions.push_back(solution);

		if(FLAG(-u)){
			SolutionInhibitor inhibitor;
			FNC negation;
			inhibitor.inhibit(solution, negation, generator);
			SolverTranslator::toSolver(negation, solver);
			TERMINAL.print("Testing unicity ");
			solver.solve();
			printer.print(solver.okay());

			if(solver.okay()){
				TERMINAL.println("NOT UNIQUE", TERMINAL.WARNING);
				Solution solution2;
				TERMINAL.println("");
				SolverTranslator::fromSolver(solver, problem, solution2);
				printer.print(solution2, 1);
				TERMINAL.println("");
				solutions.push_back(solution2);
			}
			else{
				TERMINAL.println("UNIQUE", TERMINAL.OKGREEN);
			}
		}

	}

	if(ARGV.size() >= 2){
		TERMINAL.println("Writing to file");
		filename = ARGV[1];
		FileWriter writer;
		if(!solutions.empty()){
			std::vector<Problem> problems;
			for(const Solution& solution : solutions){
				problems.push_back(problem);
				problems.back().grid = solution.grid;
			}
			writer.save(problems, filename);
		}
		else{
			writer.empty(filename);
		}
	}

	TERMINAL.println("Done");
}


void FINISH(){
    if(FLAG(-t)){
        CHRONO.stop();
        std::stringstream msg;
        msg << "> EXECUTION TIME : ";
        msg << CHRONO.sec() << " s, ";
        msg << CHRONO.msec()%1000 << " ms, ";
        msg << CHRONO.usec()%1000 << " µs.";
        TERMINAL.println(msg.str());
    }
}