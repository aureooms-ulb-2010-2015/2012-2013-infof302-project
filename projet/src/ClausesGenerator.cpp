#include "ClausesGenerator.h"

std::vector<std::vector<int>> ClausesGenerator::run(const Problem& problem){
	this->load(problem);
	return this->generateClauses();
}

ClausesGenerator::FNC ClausesGenerator::generateClauses(){
	ClausesGenerator::FNC result;
	ClausesGenerator::FNC fnc1 = generateAllHorizontalClauses();
	result.insert(result.end(), fnc1.begin(), fnc1.end());
	ClausesGenerator::FNC fnc2 = generateAllVerticalClauses();
	result.insert(result.end(), fnc2.begin(), fnc2.end());
	return result;
}

int ClausesGenerator::BOX(size_t i, size_t j){
	return i*this->_problem->size + j + 1;
}

int ClausesGenerator::LBande(size_t pos, size_t ID, size_t start){
	return this->_Loffset + pos*this->_problem->size*this->_problem->tapes + start*this->_problem->tapes + ID;
}
int ClausesGenerator::CBande(size_t pos, size_t ID, size_t start){
	return this->_Coffset + pos*this->_problem->size*this->_problem->tapes + start*this->_problem->tapes + ID;
}
int ClausesGenerator::L(size_t i, size_t ID){
	return this->_problem->rows.at(i).at(ID);
}
int ClausesGenerator::C(size_t j, size_t ID){
	return this->_problem->cols.at(j).at(ID);
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


white holes between tapes:

	A_k AND B_j => AND(-x_k) FOR ALL k < x < j FOR ALL j > k


\*---------------------------------------------------------------*/



ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseBoxes(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = start; j < start + L(pos, ID); ++j){
		lits.push_back(BOX(pos, j));
	}
	if(start + L(pos, ID) < this->_problem->size){
		lits.push_back(-BOX(pos, start + L(pos, ID)));
	}
	return this->implies(LBande(pos, ID, start), lits);
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseAtMostOne(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = start + 1; j < this->_problem->size; ++j){
		lits.push_back(-LBande(pos, ID, j));
	}
	return this->implies(LBande(pos, ID, start), lits);
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClause(size_t pos, size_t ID){
	ClausesGenerator::FNC result;

	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		ClausesGenerator::FNC fnc = generateHorizontalClauseBoxes(pos, ID, start);
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
		ClausesGenerator::FNC fnc = generateHorizontalClauseAtMostOne(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());
	}

	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseOrder(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = 0; j <= start + L(pos, ID); ++j){
		lits.push_back(-LBande(pos, ID+1, j));
	}
	return this->implies(LBande(pos, ID, start), lits);
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseOrder(size_t pos, size_t ID){
	ClausesGenerator::FNC result;
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		ClausesGenerator::FNC fnc = generateHorizontalClauseOrder(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseHoles(size_t pos, size_t ID, size_t start){
	ClausesGenerator::FNC result;
	for(size_t j = start + L(pos, ID) + 1; j < this->_problem->size; ++j){
		std::vector<int> conjonction = {LBande(pos, ID, start), LBande(pos, ID+1, j)};
		std::vector<int> lits;
		for(size_t d = start + L(pos, ID); d < j; ++d){
			lits.push_back(-BOX(pos, d));
		}
		ClausesGenerator::FNC fnc = this->implies(conjonction, lits);
		result.insert(result.end(), fnc.begin(), fnc.end());
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseHoles(size_t pos, size_t ID){
	ClausesGenerator::FNC result;
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		ClausesGenerator::FNC fnc = generateHorizontalClauseHoles(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseHolesBegin(size_t pos){
	ClausesGenerator::FNC result;
	const size_t ID = 0;
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = 0; d < start; ++d){
			lits.push_back(-BOX(pos, d));
		}
		ClausesGenerator::FNC fnc = this->implies(LBande(pos, ID, start), lits);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClauseHolesEnd(size_t pos){
	ClausesGenerator::FNC result;
	const size_t ID = this->_problem->rows.at(pos).size() - 1;
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = start + L(pos, ID); d < this->_problem->size; ++d){
			lits.push_back(-BOX(pos, d));
		}
		ClausesGenerator::FNC fnc = this->implies(LBande(pos, ID, start), lits);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateHorizontalClause(size_t pos){
	ClausesGenerator::FNC result;
	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size(); ++ID){
		ClausesGenerator::FNC fnc = generateHorizontalClause(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}

	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size() - 1; ++ID){
		ClausesGenerator::FNC fnc = generateHorizontalClauseOrder(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	ClausesGenerator::FNC fnc1 = generateHorizontalClauseHolesBegin(pos);
	result.insert(result.end(), fnc1.begin(), fnc1.end());
	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size() - 1; ++ID){
		ClausesGenerator::FNC fnc = generateHorizontalClauseHoles(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	ClausesGenerator::FNC fnc2 = generateHorizontalClauseHolesEnd(pos);
	result.insert(result.end(), fnc2.begin(), fnc2.end());
	return result;
}
ClausesGenerator::FNC ClausesGenerator::generateAllHorizontalClauses(){
	ClausesGenerator::FNC result;
	for(size_t pos = 0; pos < this->_problem->size; ++pos){
		ClausesGenerator::FNC fnc = generateHorizontalClause(pos);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}



ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseBoxes(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t i = start; i < start + C(pos, ID); ++i){
		lits.push_back(BOX(i, pos));
	}
	if(start + C(pos, ID) < this->_problem->size){
		lits.push_back(-BOX(start + C(pos, ID), pos));
	}
	return this->implies(CBande(pos, ID, start), lits);
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseAtMostOne(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t i = start + 1; i < this->_problem->size; ++i){
		lits.push_back(-CBande(pos, ID, i));
	}
	return this->implies(CBande(pos, ID, start), lits);
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClause(size_t pos, size_t ID){
	ClausesGenerator::FNC result;

	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		ClausesGenerator::FNC fnc = generateVerticalClauseBoxes(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());
	}

	for(size_t start = this->_problem->size - C(pos, ID) + 1; start < this->_problem->size; ++start){
		result.push_back({-CBande(pos, ID, start)});
	}

	Clause atLeastOne;
	for(size_t start = 0; start < this->_problem->size; ++start){
		atLeastOne.push_back(CBande(pos, ID, start));
	}
	result.push_back(atLeastOne);

	for(size_t start = 0; start < this->_problem->size - 1; ++start){
		ClausesGenerator::FNC fnc = generateVerticalClauseAtMostOne(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());
	}

	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseOrder(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t i = 0; i <= start + C(pos, ID); ++i){
		lits.push_back(-CBande(pos, ID+1, i));
	}
	return this->implies(CBande(pos, ID, start), lits);
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseOrder(size_t pos, size_t ID){
	ClausesGenerator::FNC result;
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		ClausesGenerator::FNC fnc = generateVerticalClauseOrder(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseHoles(size_t pos, size_t ID, size_t start){
	ClausesGenerator::FNC result;
	for(size_t i = start + C(pos, ID) + 1; i < this->_problem->size; ++i){
		std::vector<int> conjonction = {CBande(pos, ID, start), CBande(pos, ID+1, i)};
		std::vector<int> lits;
		for(size_t d = start + C(pos, ID); d < i; ++d){
			lits.push_back(-BOX(d, pos));
		}
		ClausesGenerator::FNC fnc = this->implies(conjonction, lits);
		result.insert(result.end(), fnc.begin(), fnc.end());
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseHoles(size_t pos, size_t ID){
	ClausesGenerator::FNC result;
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		ClausesGenerator::FNC fnc = generateVerticalClauseHoles(pos, ID, start);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseHolesBegin(size_t pos){
	ClausesGenerator::FNC result;
	const size_t ID = 0;
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = 0; d < start; ++d){
			lits.push_back(-BOX(d, pos));
		}
		ClausesGenerator::FNC fnc = this->implies(CBande(pos, ID, start), lits);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClauseHolesEnd(size_t pos){
	ClausesGenerator::FNC result;
	const size_t ID = this->_problem->cols.at(pos).size() - 1;
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = start + C(pos, ID); d < this->_problem->size; ++d){
			lits.push_back(-BOX(d, pos));
		}
		ClausesGenerator::FNC fnc = this->implies(CBande(pos, ID, start), lits);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateVerticalClause(size_t pos){
	ClausesGenerator::FNC result;
	for(size_t ID = 0; ID < this->_problem->cols.at(pos).size(); ++ID){
		ClausesGenerator::FNC fnc = generateVerticalClause(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}

	for(size_t ID = 0; ID < this->_problem->cols.at(pos).size() - 1; ++ID){
		ClausesGenerator::FNC fnc = generateVerticalClauseOrder(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}

	ClausesGenerator::FNC fnc1 = generateVerticalClauseHolesBegin(pos);
	result.insert(result.end(), fnc1.begin(), fnc1.end());
	for(size_t ID = 0; ID < this->_problem->cols.at(pos).size() - 1; ++ID){
		ClausesGenerator::FNC fnc = generateVerticalClauseHoles(pos, ID);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	ClausesGenerator::FNC fnc2 = generateVerticalClauseHolesEnd(pos);
	result.insert(result.end(), fnc2.begin(), fnc2.end());
	return result;
}

ClausesGenerator::FNC ClausesGenerator::generateAllVerticalClauses(){
	ClausesGenerator::FNC result;
	for(size_t pos = 0; pos < this->_problem->size; ++pos){
		ClausesGenerator::FNC fnc = generateVerticalClause(pos);
		result.insert(result.end(), fnc.begin(), fnc.end());		
	}
	return result;
}



ClausesGenerator::FNC ClausesGenerator::implies(int conjonction, const std::vector<int>& lits){
	ClausesGenerator::FNC result;
	for (int lit : lits){
		result.push_back({-conjonction, lit});
	}
	return result;
}

ClausesGenerator::FNC ClausesGenerator::implies(const std::vector<int>& conjonction, const std::vector<int>& lits){
	ClausesGenerator::FNC result;
	std::vector<int> negation;
	for(int lit : conjonction){
		negation.push_back(-lit);
	}
	for(int lit : lits){
		result.push_back(negation);
		result.back().push_back(lit);
	}
	return result;
}

void ClausesGenerator::load(const Problem& problem){
	this->_problem = &problem;
	this->_Loffset = this->_problem->size*this->_problem->size + 1;
	this->_Coffset = _Loffset + this->_problem->size*this->_problem->size*this->_problem->tapes;
}