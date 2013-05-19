#ifndef _CLAUSESGENERATOR_H
#define _CLAUSESGENERATOR_H

#include <functional>
#include <vector>
#include <string>
#include "FNC.h"
#include "Problem.h"


class ClausesGenerator{

private:
	const Problem* _problem;
	int _Loffset;
	int _Coffset;
	FNC* _final;

	enum Which{Rows, Columns};

	void generateClauses();

	template<Which D>
	void generateClauseBoxes(size_t pos, size_t ID, size_t start){
		std::vector<int> lits;
		for(size_t j = start; j < start + SIZE<D>(pos, ID); ++j){
			lits.push_back(BOX<D>(pos, j));
		}
		if(start + SIZE<D>(pos, ID) < this->_problem->size){
			lits.push_back(-BOX<D>(pos, start + SIZE<D>(pos, ID)));
		}
		implies(TAPE<D>(pos, ID, start), lits);
	}

	template<Which D>
	void generateClauseAtMostOne(size_t pos, size_t ID, size_t start){
		std::vector<int> lits;
		for(size_t j = start + 1; j < this->_problem->size; ++j){
			lits.push_back(-TAPE<D>(pos, ID, j));
		}
		implies(TAPE<D>(pos, ID, start), lits);
	}

	template<Which D>
	void generateClause(size_t pos, size_t ID){

		for(size_t start = 0; start + SIZE<D>(pos, ID) <= this->_problem->size; ++start){
			generateClauseBoxes<D>(pos, ID, start);
		}

		for(size_t start = this->_problem->size - SIZE<D>(pos, ID) + 1; start < this->_problem->size; ++start){
			this->_final->push_back({-TAPE<D>(pos, ID, start)});
		}

		Clause atLeastOne;
		for(size_t start = 0; start < this->_problem->size; ++start){
			atLeastOne.push_back(TAPE<D>(pos, ID, start));
		}
		this->_final->push_back(atLeastOne);

		for(size_t start = 0; start < this->_problem->size - 1; ++start){
			generateClauseAtMostOne<D>(pos, ID, start);
		}
	}

	template<Which D>
	void generateClauseOrder(size_t pos, size_t ID, size_t start){
		std::vector<int> lits;
		for(size_t j = 0; j <= start + SIZE<D>(pos, ID); ++j){
			lits.push_back(-TAPE<D>(pos, ID+1, j));
		}
		implies(TAPE<D>(pos, ID, start), lits);
	}

	template<Which D>
	void generateClauseOrder(size_t pos, size_t ID){
		for(size_t start = 0; start + SIZE<D>(pos, ID) <= this->_problem->size; ++start){
			generateClauseOrder<D>(pos, ID, start);	
		}
	}

	template<Which D>
	void generateClauseHoles(size_t pos, size_t ID, size_t start){
		for(size_t j = start + SIZE<D>(pos, ID) + 1; j < this->_problem->size; ++j){
			std::vector<int> conjonction = {TAPE<D>(pos, ID, start), TAPE<D>(pos, ID+1, j)};
			std::vector<int> lits;
			for(size_t d = start + SIZE<D>(pos, ID); d < j; ++d){
				lits.push_back(-BOX<D>(pos, d));
			}
			implies(conjonction, lits);
		}
	}

	template<Which D>
	void generateClauseHoles(size_t pos, size_t ID){
		for(size_t start = 0; start + SIZE<D>(pos, ID) <= this->_problem->size; ++start){
			generateClauseHoles<D>(pos, ID, start);		
		}
	}

	template<Which D>
	void generateClauseHolesBegin(size_t pos){
		const size_t ID = 0;
		for(size_t start = 0; start + SIZE<D>(pos, ID) <= this->_problem->size; ++start){
			std::vector<int> lits;
			for(size_t d = 0; d < start; ++d){
				lits.push_back(-BOX<D>(pos, d));
			}
			implies(TAPE<D>(pos, ID, start), lits);	
		}
	}

	template<Which D>
	void generateClauseHolesEnd(size_t pos){
		const size_t ID = TAPES<D>().at(pos).size() - 1;
		for(size_t start = 0; start + SIZE<D>(pos, ID) <= this->_problem->size; ++start){
			std::vector<int> lits;
			for(size_t d = start + SIZE<D>(pos, ID); d < this->_problem->size; ++d){
				lits.push_back(-BOX<D>(pos, d));
			}
			implies(TAPE<D>(pos, ID, start), lits);	
		}
	}

	template<Which D>
	void generateClause(size_t pos){
		for(size_t ID = 0; ID < TAPES<D>().at(pos).size(); ++ID){
			generateClause<D>(pos, ID);
		}

		for(size_t ID = 0; ID < TAPES<D>().at(pos).size() - 1; ++ID){
			generateClauseOrder<D>(pos, ID);
		}
		generateClauseHolesBegin<D>(pos);
		for(size_t ID = 0; ID < TAPES<D>().at(pos).size() - 1; ++ID){
			generateClauseHoles<D>(pos, ID);		
		}
		generateClauseHolesEnd<D>(pos);
	}

	template<Which D>
	void generateAllClauses(){
		for(size_t pos = 0; pos < this->_problem->size; ++pos){
			generateClause<D>(pos);		
		}
	}

	void implies(int conjonction, const std::vector<int>& lits);
	void implies(const std::vector<int>& conjonction, const std::vector<int>& lits);
	virtual void load(const Problem&);


public:
	virtual void run(const Problem&, FNC& clauses);

	template<Which D>
	inline int BOX(size_t i, size_t j) const{
		if(D == Rows) return RBOX(i, j);
		else return CBOX(i, j);
	}
	template<Which D>
	inline int TAPE(size_t pos, size_t ID, size_t start) const{
		if(D == Rows) return RTAPE(pos, ID, start);
		else return CTAPE(pos, ID, start);
	}
	template<Which D>
	inline int SIZE(size_t i, size_t ID) const{
		if(D == Rows) return R(i, ID);
		else return C(i, ID);
	}
	template<Which D>
	inline const std::vector<std::vector<int>>& TAPES() const{
		if(D == Rows) return ROWS();
		else return COLS();
	}

	int RBOX(size_t i, size_t j) const;
	int CBOX(size_t j, size_t i) const;
	int RTAPE(size_t pos, size_t ID, size_t start) const;
	int CTAPE(size_t pos, size_t ID, size_t start) const;
	int R(size_t i, size_t ID) const;
	int C(size_t j, size_t ID) const;
	const std::vector<std::vector<int>>& ROWS() const;
	const std::vector<std::vector<int>>& COLS() const;

};


#endif // _CLAUSESGENERATOR_H
