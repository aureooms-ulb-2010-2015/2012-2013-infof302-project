#include "ClausesGenerator.h"

std::vector<std::vector<int>> ClausesGenerator::run(const Problem& problem){
	this->load(problem);
	return this->generateClauses();
}

FNC ClausesGenerator::generateClauses(){
	FNC clauses;
	FNC fnc1 = generateAllHorizontalClauses();
	result.insert(result.end(), fnc1.begin(), fnc1.end());
	FNC fnc2 = generateAllVerticalClauses();
	result.insert(result.end(), fnc2.begin(), fnc2.end());
	return clauses;
}

int ClausesGenerator::BOX(size_t i, size_t j){
	return i*this->_problem->size + j;
}

int ClausesGenerator::LBande(size_t pos, size_t ID, size_t start){
	return this->_Loffset + pos*this->_problem->size*this->_problem->max + start*this->_problem->max + ID;
}
int ClausesGenerator::CBande(size_t pos, size_t ID, size_t start){
	return this->_Coffset + pos*this->_problem->size*this->_problem->max + start*this->_problem->max + ID;
}
int ClausesGenerator::L(size_t i, size_t ID){
	return this->_problem->rows.at(i).at(ID);
}
int ClausesGenerator::C(size_t j, size_t ID){
	return this->_problem->cols.at(i).at(ID);
}


/*---------------------------------------------------------------*\

given

a, b, c, d, ... := start positions
1, 2, 3, 4, ... := boxes

start position implies pattern:
	a => 1, 2, 3, ..., -n    == (-a or 1) and (-a or 2) and (-a or 3) and ... and  (-a or -n)
	b => 2, 3, 4, ..., -(n+1)
	c => 3, 4, 5, ..., -(n+2)
	d => 4, 5, 6, ..., -(n+3)

at least one start position:
	a or b or c or d or ...

at  most one start position:
	a => -b and -c and -d and ...
	b => -c and -d and ...
	c => -d and ...
	d =>  ...
	... => ...

order matters:

	A_k => AND (-B_j) FOR ALL j <= k+1
	B_k => AND (-C_j) FOR ALL j <= k+1


\*---------------------------------------------------------------*/



FNC ClausesGenerator::generateHorizontalClauseBoxes(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = start; j < start + L(pos, ID); ++j){
		lits.push_back(BOX(pos, j));
	}
	if(start + L(pos, ID) < this->_problem->size){
		lits.push_back(-BOX(pos, start + L(pos, ID)));
	}
	return this->implies(LBande(pos, ID, start), lits);
}

FNC ClausesGenerator::generateHorizontalClauseAtMostOne(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = start + 1; j < this->_problem->size; ++j){
		lits.push_back(-LBande(pos, ID, j));
	}
	return this->implies(LBande(pos, ID, start), lits);
}

FNC ClausesGenerator::generateHorizontalClause(size_t pos, size_t ID){
	FNC result;

	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		FNC fnc = generateHorizontalClauseBoxes(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());
	}

	for(size_t start = this->_problem->size - L(pos, ID) + 1; start < this->_problem->size; ++start){
		result.push_back({-LBande(pos, ID, start)});
	}

	Clause atLeastOne;
	for(size_t start = 0; start < this->_problem->size; ++start){
		atLeastOne.push_back(LBande(pos, ID, start));
	}
	result.push_back(atLeastOne);

	for(size_t start = 0; start < this->_problem->size - 1; ++start){
		FNC fnc = generateHorizontalClauseAtMostOne(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());
	}

	return result;
}

FNC ClausesGenerator::generateHorizontalClauseOrder(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = 0; j <= start + L(pos, ID); ++j){
		lits.push_back(-LBande(pos, ID+1, j));
	}
	return this->implies(LBande(pos, ID, start), lits);
}

FNC ClausesGenerator::generateHorizontalClauseOrder(size_t pos, size_t ID){
	FNC result;
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		FNC fnc = generateHorizontalClauseOrder(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

FNC ClausesGenerator::generateHorizontalClause(size_t pos){
	FNC result;
	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size(); ++ID){
		FNC fnc = generateHorizontalClause(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}

	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size() - 1; ++ID){
		FNC fnc = generateHorizontalClauseOrder(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}
FNC ClausesGenerator::generateAllHorizontalClauses(){
	FNC result;
	for(size_t pos = 0; pos < this->_problem->size; ++pos){
		FNC fnc = generateHorizontalClause(pos);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}


FNC ClausesGenerator::generateVerticalClause(size_t pos, size_t ID, size_t start){
	FNC result;
	return result;
}
FNC ClausesGenerator::generateVerticalClause(size_t pos, size_t ID){
	FNC result;
	return result;
}














FNC ClausesGenerator::implies(int condition, std::vector<int> lits){
	FNC result;
	for (int lit : lits){
		result.push_back({-condition, lit});
	}
	return result;
}

void load(const Problem& problem){
	this->_problem = &problem;
	this->_Loffset = (this->_problem->size*this->_problem->size);
	this->_Coffset = _Loffset + this->_problem->size*this->_problem->size*this->_problem->max);
}