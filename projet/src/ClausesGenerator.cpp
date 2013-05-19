#include "ClausesGenerator.h"

void ClausesGenerator::run(const Problem& problem, FNC& clauses){
	this->_final = &clauses;
	this->load(problem);
	this->generateClauses();
}

void ClausesGenerator::generateClauses(){
	generateAllClauses<ClausesGenerator::Rows>();
	generateAllClauses<ClausesGenerator::Columns>();
}

inline int ClausesGenerator::RBOX(size_t i, size_t j) const{
	return i*this->_problem->size + j + 1;
}

inline int ClausesGenerator::CBOX(size_t j, size_t i) const{
	return i*this->_problem->size + j + 1;
}

inline int ClausesGenerator::RTAPE(size_t pos, size_t ID, size_t start) const{
	return this->_Loffset + pos*this->_problem->size*this->_problem->tapes + start*this->_problem->tapes + ID;
}
inline int ClausesGenerator::CTAPE(size_t pos, size_t ID, size_t start) const{
	return this->_Coffset + pos*this->_problem->size*this->_problem->tapes + start*this->_problem->tapes + ID;
}

inline int ClausesGenerator::R(size_t i, size_t ID) const{
	return this->_problem->rows.at(i).at(ID);
}
inline int ClausesGenerator::C(size_t j, size_t ID) const{
	return this->_problem->cols.at(j).at(ID);
}

inline const std::vector<std::vector<int>>& ClausesGenerator::ROWS() const{
	return this->_problem->rows;
}
inline const std::vector<std::vector<int>>& ClausesGenerator::COLS() const{
	return this->_problem->cols;
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

inline void ClausesGenerator::implies(int conjonction, const std::vector<int>& lits){
	for (int lit : lits){
		this->_final->push_back({-conjonction, lit});
	}
}

inline void ClausesGenerator::implies(const std::vector<int>& conjonction, const std::vector<int>& lits){
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