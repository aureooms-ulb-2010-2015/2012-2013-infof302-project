#include "ClausesGenerator.h"

void ClausesGenerator::run(const Problem& problem, FNC& clauses){
	this->_final = &clauses;
	this->load(problem);
	return this->generateClauses();
}

void ClausesGenerator::generateClauses(){
	generateAllHorizontalClauses();
	generateAllVerticalClauses();
}

int ClausesGenerator::BOX(size_t i, size_t j) const{
	return i*this->_problem->size + j + 1;
}

int ClausesGenerator::LBande(size_t pos, size_t ID, size_t start) const{
	return this->_Loffset + pos*this->_problem->size*this->_problem->tapes + start*this->_problem->tapes + ID;
}
int ClausesGenerator::CBande(size_t pos, size_t ID, size_t start) const{
	return this->_Coffset + pos*this->_problem->size*this->_problem->tapes + start*this->_problem->tapes + ID;
}
int ClausesGenerator::L(size_t i, size_t ID) const{
	return this->_problem->rows.at(i).at(ID);
}
int ClausesGenerator::C(size_t j, size_t ID) const{
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



void ClausesGenerator::generateHorizontalClauseBoxes(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = start; j < start + L(pos, ID); ++j){
		lits.push_back(BOX(pos, j));
	}
	if(start + L(pos, ID) < this->_problem->size){
		lits.push_back(-BOX(pos, start + L(pos, ID)));
	}
	implies(LBande(pos, ID, start), lits);
}

void ClausesGenerator::generateHorizontalClauseAtMostOne(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = start + 1; j < this->_problem->size; ++j){
		lits.push_back(-LBande(pos, ID, j));
	}
	implies(LBande(pos, ID, start), lits);
}

void ClausesGenerator::generateHorizontalClause(size_t pos, size_t ID){

	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		generateHorizontalClauseBoxes(pos, ID, start);
	}

	for(size_t start = this->_problem->size - L(pos, ID) + 1; start < this->_problem->size; ++start){
		this->_final->push_back({-LBande(pos, ID, start)});
	}

	Clause atLeastOne;
	for(size_t start = 0; start < this->_problem->size; ++start){
		atLeastOne.push_back(LBande(pos, ID, start));
	}
	this->_final->push_back(atLeastOne);

	for(size_t start = 0; start < this->_problem->size - 1; ++start){
		generateHorizontalClauseAtMostOne(pos, ID, start);
	}
}

void ClausesGenerator::generateHorizontalClauseOrder(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t j = 0; j <= start + L(pos, ID); ++j){
		lits.push_back(-LBande(pos, ID+1, j));
	}
	implies(LBande(pos, ID, start), lits);
}

void ClausesGenerator::generateHorizontalClauseOrder(size_t pos, size_t ID){
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		generateHorizontalClauseOrder(pos, ID, start);	
	}
}

void ClausesGenerator::generateHorizontalClauseHoles(size_t pos, size_t ID, size_t start){
	for(size_t j = start + L(pos, ID) + 1; j < this->_problem->size; ++j){
		std::vector<int> conjonction = {LBande(pos, ID, start), LBande(pos, ID+1, j)};
		std::vector<int> lits;
		for(size_t d = start + L(pos, ID); d < j; ++d){
			lits.push_back(-BOX(pos, d));
		}
		implies(conjonction, lits);
	}
}

void ClausesGenerator::generateHorizontalClauseHoles(size_t pos, size_t ID){
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		generateHorizontalClauseHoles(pos, ID, start);		
	}
}

void ClausesGenerator::generateHorizontalClauseHolesBegin(size_t pos){
	const size_t ID = 0;
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = 0; d < start; ++d){
			lits.push_back(-BOX(pos, d));
		}
		implies(LBande(pos, ID, start), lits);	
	}
}

void ClausesGenerator::generateHorizontalClauseHolesEnd(size_t pos){
	const size_t ID = this->_problem->rows.at(pos).size() - 1;
	for(size_t start = 0; start + L(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = start + L(pos, ID); d < this->_problem->size; ++d){
			lits.push_back(-BOX(pos, d));
		}
		implies(LBande(pos, ID, start), lits);	
	}
}

void ClausesGenerator::generateHorizontalClause(size_t pos){
	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size(); ++ID){
		generateHorizontalClause(pos, ID);
	}

	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size() - 1; ++ID){
		generateHorizontalClauseOrder(pos, ID);
	}
	generateHorizontalClauseHolesBegin(pos);
	for(size_t ID = 0; ID < this->_problem->rows.at(pos).size() - 1; ++ID){
		generateHorizontalClauseHoles(pos, ID);		
	}
	generateHorizontalClauseHolesEnd(pos);
}
void ClausesGenerator::generateAllHorizontalClauses(){
	for(size_t pos = 0; pos < this->_problem->size; ++pos){
		generateHorizontalClause(pos);		
	}
}



void ClausesGenerator::generateVerticalClauseBoxes(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t i = start; i < start + C(pos, ID); ++i){
		lits.push_back(BOX(i, pos));
	}
	if(start + C(pos, ID) < this->_problem->size){
		lits.push_back(-BOX(start + C(pos, ID), pos));
	}
	implies(CBande(pos, ID, start), lits);
}

void ClausesGenerator::generateVerticalClauseAtMostOne(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t i = start + 1; i < this->_problem->size; ++i){
		lits.push_back(-CBande(pos, ID, i));
	}
	implies(CBande(pos, ID, start), lits);
}

void ClausesGenerator::generateVerticalClause(size_t pos, size_t ID){

	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		generateVerticalClauseBoxes(pos, ID, start);
	}

	for(size_t start = this->_problem->size - C(pos, ID) + 1; start < this->_problem->size; ++start){
		this->_final->push_back({-CBande(pos, ID, start)});
	}

	Clause atLeastOne;
	for(size_t start = 0; start < this->_problem->size; ++start){
		atLeastOne.push_back(CBande(pos, ID, start));
	}
	this->_final->push_back(atLeastOne);

	for(size_t start = 0; start < this->_problem->size - 1; ++start){
		generateVerticalClauseAtMostOne(pos, ID, start);
	}
}

void ClausesGenerator::generateVerticalClauseOrder(size_t pos, size_t ID, size_t start){
	std::vector<int> lits;
	for(size_t i = 0; i <= start + C(pos, ID); ++i){
		lits.push_back(-CBande(pos, ID+1, i));
	}
	implies(CBande(pos, ID, start), lits);
}

void ClausesGenerator::generateVerticalClauseOrder(size_t pos, size_t ID){
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		generateVerticalClauseOrder(pos, ID, start);	
	}
}

void ClausesGenerator::generateVerticalClauseHoles(size_t pos, size_t ID, size_t start){
	for(size_t i = start + C(pos, ID) + 1; i < this->_problem->size; ++i){
		std::vector<int> conjonction = {CBande(pos, ID, start), CBande(pos, ID+1, i)};
		std::vector<int> lits;
		for(size_t d = start + C(pos, ID); d < i; ++d){
			lits.push_back(-BOX(d, pos));
		}
		implies(conjonction, lits);
	}
}

void ClausesGenerator::generateVerticalClauseHoles(size_t pos, size_t ID){
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		generateVerticalClauseHoles(pos, ID, start);	
	}
}

void ClausesGenerator::generateVerticalClauseHolesBegin(size_t pos){
	const size_t ID = 0;
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = 0; d < start; ++d){
			lits.push_back(-BOX(d, pos));
		}
		implies(CBande(pos, ID, start), lits);	
	}
}

void ClausesGenerator::generateVerticalClauseHolesEnd(size_t pos){
	const size_t ID = this->_problem->cols.at(pos).size() - 1;
	for(size_t start = 0; start + C(pos, ID) <= this->_problem->size; ++start){
		std::vector<int> lits;
		for(size_t d = start + C(pos, ID); d < this->_problem->size; ++d){
			lits.push_back(-BOX(d, pos));
		}
		implies(CBande(pos, ID, start), lits);	
	}
}

void ClausesGenerator::generateVerticalClause(size_t pos){
	for(size_t ID = 0; ID < this->_problem->cols.at(pos).size(); ++ID){
		generateVerticalClause(pos, ID);
	}

	for(size_t ID = 0; ID < this->_problem->cols.at(pos).size() - 1; ++ID){
		generateVerticalClauseOrder(pos, ID);	
	}

	generateVerticalClauseHolesBegin(pos);
	for(size_t ID = 0; ID < this->_problem->cols.at(pos).size() - 1; ++ID){
		generateVerticalClauseHoles(pos, ID);	
	}
	generateVerticalClauseHolesEnd(pos);
}

void ClausesGenerator::generateAllVerticalClauses(){
	for(size_t pos = 0; pos < this->_problem->size; ++pos){
		generateVerticalClause(pos);
	}
}



void ClausesGenerator::implies(int conjonction, const std::vector<int>& lits){
	for (int lit : lits){
		this->_final->push_back({-conjonction, lit});
	}
}

void ClausesGenerator::implies(const std::vector<int>& conjonction, const std::vector<int>& lits){
	std::vector<int> negation;
	for(int lit : conjonction){
		negation.push_back(-lit);
	}
	for(int lit : lits){
		this->_final->push_back(negation);
		this->_final->back().push_back(lit);
	}
}

void ClausesGenerator::load(const Problem& problem){
	this->_problem = &problem;
	this->_Loffset = this->_problem->size*this->_problem->size + 1;
	this->_Coffset = _Loffset + this->_problem->size*this->_problem->size*this->_problem->tapes;
}