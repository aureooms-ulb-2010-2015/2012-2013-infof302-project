#ifndef _CLAUSESGENERATOR_H
#define _CLAUSESGENERATOR_H

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

	virtual void generateClauses();
	virtual void generateHorizontalClauseAtMostOne(size_t pos, size_t ID, size_t start);
	virtual void generateHorizontalClauseBoxes(size_t pos, size_t ID, size_t start);
	virtual void generateHorizontalClauseOrder(size_t pos, size_t ID, size_t start);
	virtual void generateHorizontalClauseOrder(size_t pos, size_t ID);
	virtual void generateHorizontalClauseHoles(size_t pos, size_t ID, size_t start);
	virtual void generateHorizontalClauseHoles(size_t pos, size_t ID);
	virtual void generateHorizontalClauseHolesBegin(size_t pos);
	virtual void generateHorizontalClauseHolesEnd(size_t pos);
	virtual void generateHorizontalClause(size_t pos, size_t ID);
	virtual void generateHorizontalClause(size_t pos);
	virtual void generateAllHorizontalClauses();
	virtual void generateVerticalClauseAtMostOne(size_t pos, size_t ID, size_t start);
	virtual void generateVerticalClauseBoxes(size_t pos, size_t ID, size_t start);
	virtual void generateVerticalClauseOrder(size_t pos, size_t ID, size_t start);
	virtual void generateVerticalClauseOrder(size_t pos, size_t ID);
	virtual void generateVerticalClauseHoles(size_t pos, size_t ID, size_t start);
	virtual void generateVerticalClauseHoles(size_t pos, size_t ID);
	virtual void generateVerticalClauseHolesBegin(size_t pos);
	virtual void generateVerticalClauseHolesEnd(size_t pos);
	virtual void generateVerticalClause(size_t pos, size_t ID);
	virtual void generateVerticalClause(size_t pos);
	virtual void generateAllVerticalClauses();
	virtual void implies(int conjonction, const std::vector<int>& lits);
	virtual void implies(const std::vector<int>& conjonction, const std::vector<int>& lits);
	virtual void load(const Problem&);


public:
	virtual void run(const Problem&, FNC& clauses);
	
	virtual int BOX(size_t i, size_t j) const;
	virtual int LBande(size_t pos, size_t ID, size_t start) const;
	virtual int CBande(size_t pos, size_t ID, size_t start) const;
	virtual int L(size_t i, size_t ID) const;
	virtual int C(size_t j, size_t ID) const;
};


#endif // _CLAUSESGENERATOR_H
