#include "PrettyPrinter.h"
#include "Terminal.h"
#include <iostream>
#include <algorithm>
#include <iterator>


void PrettyPrinter::print(const Problem& problem){
	std::cout << problem.size << " " << problem.tapes << std::endl;
	for(const std::vector<int>& col : problem.cols){
		std::copy(col.begin(), col.end(),
		std::ostream_iterator<int>(std::cout, " "));
		std::cout <<  std::endl;
	}
	for(const std::vector<int>& row : problem.rows){
		std::copy(row.begin(), row.end(),
		std::ostream_iterator<int>(std::cout, " "));
		std::cout <<  std::endl;
	}
	for(const std::vector<int>& line : problem.grid){
		std::copy(line.begin(), line.end(),
		std::ostream_iterator<int>(std::cout, " "));
		std::cout <<  std::endl;
	}
}
void PrettyPrinter::print(const Solution& solution, size_t indent){
	Terminal terminal;
	for(const std::vector<int>& line : solution.grid){
		for(size_t i = 0; i < indent; ++i){
			terminal.print(" ");
		}
		for(size_t i = 0; i < line.size(); ++i){
			if(line.at(i) == 1){
				terminal.print("B", terminal.OKGREEN);
			}
			else if(line.at(i) == -1){
				terminal.print("W", terminal.OKBLUE);
			}
			else{
				terminal.print("U", terminal.WARNING);
			}
			terminal.print(" ");
		}
		terminal.print("\n");
	}
}

void PrettyPrinter::print(const lbool& ret){
	Terminal terminal;
	if(ret == l_True) terminal.println("=> SAT", terminal.OKGREEN);
	else if(ret == l_False) terminal.println("=> UNSAT", terminal.WARNING);
	else terminal.println("=> INDET", terminal.HEADER);
}

void PrettyPrinter::print(const std::vector<std::vector<int>>& grid){
	for(const std::vector<int>& line : grid){
		std::copy(line.begin(), line.end(),
		std::ostream_iterator<int>(std::cout, " "));
		std::cout <<  std::endl;
	}
}