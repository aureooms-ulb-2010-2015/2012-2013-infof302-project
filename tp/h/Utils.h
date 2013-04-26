#ifndef __UTILS_HPP__
	#define __UTILS_HPP__

#include <string>
#include <sstream>
#include <vector>
#include <iostream>//TODO remove

class Utils{
	private:
		static  int countVariables(std::vector<std::vector<int>> clauses);
		static  std::string clauseToString(std::vector<int> clause);
	
	public:
		static std::vector<std::vector<char>> MINES_SWEEPER_1;
		static std::vector<std::vector<char>> MINES_SWEEPER_2;
		static std::vector<std::vector<char>> MINES_SWEEPER_3;
		static  std::string toDIMACS(std::vector<std::vector<int>> clauses);
};

#endif

