#include "ClausesGenerator.h"
#include <sstream>

std::vector<std::vector<int>> ClausesGenerator::run(std::vector<std::vector<char>> table){
	std::vector<std::vector<int>> result;
	
	int size = table.size();
	
	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			if(table[i][j] == '?') continue;
			if(table[i][j] == 'M'){
				//it's a mine!
				std::vector<int> clause = {i*size+j+1};
				result.push_back(clause);
				continue;
			}
			//it's an indicator
			int mines = 0;
			std::stringstream ss;
			ss << table[i][j];
			ss >> mines;
			
			//thus not a mine
			std::vector<int> clause = {-(i*size+j+1)};
			result.push_back(clause);
			
			
			std::vector<int> neighbours;
			
			for(int k = -1; k <= 1; ++k){
				for(int l = -1; l <= 1; ++l){
					if(k == 0 && l == 0) continue;
					int m = i+k;
					int n = j+l;
					if(0 <= m && m < size && 0 <= n && n < size){
						if(table[m][n] == 'M'){
							--mines;
							continue;
						}
						if(table[m][n] != '?') continue;
						neighbours.push_back(m*size+n+1);
					}
				}
			}
			
			
			std::vector<std::vector<int>> clauses = generateClauses(neighbours, mines);
			result.insert(result.end(), clauses.begin(), clauses.end());
		}
	}
	return result;
}

std::vector<std::vector<int>> ClausesGenerator::generateClauses(std::vector<int> neighbours, int mines){
	std::vector<std::vector<int>> clauses;
	if(neighbours.size() > 0){
		//init
		clauses.push_back(std::vector<int>());
		
		generateClausesRecursively(clauses, neighbours, mines, 0);
	}
	return clauses;
}

/*
 * Returns FNC clauses by enumerating non working combinations
 *
 */
void ClausesGenerator::generateClausesRecursively(std::vector<std::vector<int>>& clauses, std::vector<int> neighbours, int mines, size_t offset){

	//base case
	
	if(offset == neighbours.size()){
		return;
	}

	//clause is currently a valid one => invalidate it
	if(offset == neighbours.size()-1){
		if(mines == 0){ //already 0 mine left => take one
			clauses.back().push_back(-neighbours[offset]);
			generateClausesRecursively(clauses, neighbours, mines-1, offset+1);
			return;
		}
		else if(mines == 1){ //1 mine left => leave it
			clauses.back().push_back(neighbours[offset]);
			generateClausesRecursively(clauses, neighbours, mines, offset+1);
			return;
		}
	}

	std::vector<int> copy;
	if(offset + mines <= neighbours.size()){
		copy = clauses.back();
	}

	//neighbour is a mine (in a non-valid combination)
	
	clauses.back().push_back(-neighbours[offset]);
	generateClausesRecursively(clauses, neighbours, mines-1, offset+1);
	
	if(offset + mines <= neighbours.size()){
		clauses.push_back(copy);
		
		//neighbour is not a mine (in a non-valid combination)
		
		clauses.back().push_back(neighbours[offset]);
		generateClausesRecursively(clauses, neighbours, mines, offset+1);
	}
	
	return; //MOTHAFUCKA
}
