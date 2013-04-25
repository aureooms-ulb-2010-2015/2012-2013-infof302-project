
#include "Utils.h"
#include "ClausesGenerator.h"

int main(/*int argn,char ** args*/){
	for(int i = 0; i < 5; ++i){
		for(int j = 0; j < 5; ++j){
			std::cout << Utils::MINES_SWEEPER_1[i][j] << " ";
		}
		std::cout << "\n";
	}
	
	std::vector<std::vector<int>> clauses;
	clauses.push_back({1,2,5,7});
	clauses.push_back({-1});
	
	
	Utils::toDIMACS(clauses);
	std::cout << Utils::toDIMACS(clauses);
	return 0;
}
