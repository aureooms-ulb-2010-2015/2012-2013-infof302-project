#include "Utils.h"
#include <cstring>

std::vector<std::vector<char>> Utils::MINES_SWEEPER_1 = {
			{'0','0','0','1','1'},
			{'0','0','0','2','?'},
			{'0','0','0','2','?'},
			{'1','1','0','1','1'},
			{'?','1','0','0','0'}
};

std::vector<std::vector<char>> Utils::MINES_SWEEPER_2 = {
			{'?','?','?','?','?','?'},
			{'?','2','2','2','2','?'},
			{'?','2','0','0','2','?'},
			{'?','2','0','0','2','?'},
			{'?','2','2','2','2','?'},
			{'?','?','?','?','?','?'}
};
std::vector<std::vector<char>> Utils::MINES_SWEEPER_3 = {
			{'0','0','0','0','0','0','0','0'},
			{'0','1','1','1','1','1','1','0'},
			{'0','1','M','2','2','M','1','0'},
			{'0','1','2','?','?','2','1','0'},
			{'0','1','2','?','?','2','1','0'},
			{'0','1','M','2','2','M','1','0'},
			{'0','1','1','1','1','1','1','0'},
			{'0','0','0','0','0','0','0','0'}
};


int Utils::countVariables(std::vector<std::vector<int>> clauses){
	int res = 0;
	for(size_t i = 0; i < clauses.size(); ++i){
		std::vector<int> clause = clauses[i];
		for (size_t j = 0; j < clause.size(); ++j){
			int val = clause[j];
			if(val < 0) val = -val;
			if(val > res) res = val;
		}
	}
	return res;
}
std::string Utils::clauseToString(std::vector<int> clause){
	std::string res;
	std::ostringstream oss;
	for (size_t j = 0; j < clause.size(); ++j){
		oss << clause[j];
		res.append(oss.str());
		oss.clear();
		oss.str("");
		res.append(" ");
	}
	res.append("0");
	return res;
}

std::string Utils::toDIMACS(std::vector<std::vector<int>> clauses){
	int varCount = countVariables(clauses);
	std::ostringstream oss;
	std::string res("p cnf ");
	oss << varCount;
	res.append(oss.str());
	oss.clear();
	oss.str("");
	res.append(" ");
	oss << clauses.size();
	res.append(oss.str());
	oss.clear();
	oss.str("");
	res.append("\n");
	for(size_t i = 0; i < clauses.size(); ++i){
		std::vector<int> clause = clauses[i];
		res.append(Utils::clauseToString(clause));
		res.append("\n");
	}
	return res;
}

void Utils::print(const std::vector<std::vector<char>>& table){
	for(size_t i = 0; i < table.size(); ++i){
		for(size_t j = 0; j < table.front().size(); ++j){
			std::cout << table[i][j] << " ";
		}
		std::cout << "\n";
	}
}