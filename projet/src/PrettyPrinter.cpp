#include "PrettyPrinter.h"
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
void PrettyPrinter::print(const Solution& solution){

	for(const std::vector<int>& line : solution.grid){
		std::copy(line.begin(), line.end(),
		std::ostream_iterator<int>(std::cout, " "));
		std::cout <<  std::endl;
	}
}

void PrettyPrinter::print(const lbool& ret){
	printf(ret == l_True ? "SATISFIABLE\n" : ret == l_False ? "UNSATISFIABLE\n" : "INDETERMINATE\n");
}

void PrettyPrinter::print(const std::vector<std::vector<int>>& grid){
	for(const std::vector<int>& line : grid){
		std::copy(line.begin(), line.end(),
		std::ostream_iterator<int>(std::cout, " "));
		std::cout <<  std::endl;
	}
}