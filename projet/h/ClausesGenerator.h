#ifndef _CLAUSESGENERATOR_H
#define _CLAUSESGENERATOR_H

#include <vector>
#include <string>
#include "Problem.h"


class ClausesGenerator{

public:
	typedef std::vector<int> Clause;
	typedef std::vector<Clause> FNC;
	
private:
	const Problem* _problem;
	int _Loffset;
	int _Coffset;

	virtual FNC generateClauses();
	virtual int BOX(size_t i, size_t j);
	virtual int LBande(size_t pos, size_t ID, size_t start);
	virtual int CBande(size_t pos, size_t ID, size_t start);
	virtual int L(size_t i, size_t ID);
	virtual int C(size_t j, size_t ID);
	virtual FNC generateHorizontalClauseAtMostOne(size_t pos, size_t ID, size_t start);
	virtual FNC generateHorizontalClauseBoxes(size_t pos, size_t ID, size_t start);
	virtual FNC generateHorizontalClauseOrder(size_t pos, size_t ID, size_t start);
	virtual FNC generateHorizontalClauseOrder(size_t pos, size_t ID);
	virtual FNC generateHorizontalClauseHoles(size_t pos, size_t ID, size_t start);
	virtual FNC generateHorizontalClauseHoles(size_t pos, size_t ID);
	virtual FNC generateHorizontalClauseHolesBegin(size_t pos);
	virtual FNC generateHorizontalClauseHolesEnd(size_t pos);
	virtual FNC generateHorizontalClause(size_t pos, size_t ID);
	virtual FNC generateHorizontalClause(size_t pos);
	virtual FNC generateAllHorizontalClauses();
	virtual FNC generateVerticalClauseAtMostOne(size_t pos, size_t ID, size_t start);
	virtual FNC generateVerticalClauseBoxes(size_t pos, size_t ID, size_t start);
	virtual FNC generateVerticalClauseOrder(size_t pos, size_t ID, size_t start);
	virtual FNC generateVerticalClauseOrder(size_t pos, size_t ID);
	virtual FNC generateVerticalClauseHoles(size_t pos, size_t ID, size_t start);
	virtual FNC generateVerticalClauseHoles(size_t pos, size_t ID);
	virtual FNC generateVerticalClauseHolesBegin(size_t pos);
	virtual FNC generateVerticalClauseHolesEnd(size_t pos);
	virtual FNC generateVerticalClause(size_t pos, size_t ID);
	virtual FNC generateVerticalClause(size_t pos);
	virtual FNC generateAllVerticalClauses();
	virtual FNC implies(int conjonction, const std::vector<int>& lits);
	virtual FNC implies(const std::vector<int>& conjonction, const std::vector<int>& lits);
	virtual void load(const Problem&);


public:
	virtual FNC run(const Problem&);
};


#endif // _CLAUSESGENERATOR_H
